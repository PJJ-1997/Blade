#include "Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
	: CurrentStamina(100.0f), // �ʱ� ���¹̳� ����
	MaxStamina(100.0f),    // �ִ� ���¹̳� ����
	StaminaRecoveryRate(5.0f), // ���¹̳� ȸ�� �ӵ� ����
	DashStaminaCost(20.0f)// ��� �� ���¹̳� �Ҹ�
{
	CurrentPitch = 0.0f;
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
    // InGame���� Parent�� ���� �ü� ���� : ������(X)
    OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
    CheckFalse(bCanMove);      // PichYawRoll
    FRotator rotator   = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
    FVector  direction = FQuat(rotator).GetForwardVector();

    if (bTopViewCamera)
        direction = FVector::XAxisVector;

    OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
    CheckFalse(bCanMove);      // PichYawRoll
    FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
    FVector  direction = FQuat(rotator).GetRightVector();

    if (bTopViewCamera)
        direction = FVector::YAxisVector;
    OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
    CheckTrue(bFixedCamera);
    OwnerCharacter->AddControllerYawInput(InAxis*HorizontalLook*GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
    CheckTrue(bFixedCamera);
	APlayerController* PlayerController = Cast<APlayerController>(OwnerCharacter->Controller);
	if (PlayerController)
	{
		// Adjust the current pitch value by the input
		CurrentPitch = FMath::Clamp(CurrentPitch - InAxis * VerticalLook * GetWorld()->GetDeltaSeconds(), MinPitchAngle, MaxPitchAngle);

		// Get the current control rotation and set the new pitch
		FRotator NewRotation = PlayerController->GetControlRotation();
		NewRotation.Pitch = CurrentPitch;
		PlayerController->SetControlRotation(NewRotation);
	}
    //OwnerCharacter->AddControllerPitchInput(InAxis*VerticalLook*GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::EnableControlRotation()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;

		if (UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			MovementComponent->bOrientRotationToMovement = false;
		}
	}
   // OwnerCharacter->bUseControllerRotationYaw = true;
   // OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

}

void UCMovementComponent::DisableControlRotation()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->bUseControllerRotationYaw = false;

		if (UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement())
		{
			MovementComponent->bOrientRotationToMovement = true;
		}
	}
   // OwnerCharacter->bUseControllerRotationYaw = false;
   // OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnWalk()
{
    SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnRun()
{
    SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnSprint()
{
    SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];

}

bool UCMovementComponent::IsFalling() const
{
	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();

	if (MovementComponent)
	{
		
		return MovementComponent->Velocity.Z < 0.0f;
	}

	return false;
}

void UCMovementComponent::RecoverStamina(float DeltaTime)
{
	CurrentStamina += StaminaRecoveryRate * DeltaTime;
	// ���¹̳��� �ִ�ġ�� �ʰ����� �ʵ��� �մϴ�.
	CurrentStamina = FMath::Min(CurrentStamina, MaxStamina);
}

void UCMovementComponent::EnableCollision()
{
	OwnerCharacter->SetActorEnableCollision(true);
}

void UCMovementComponent::SetStamina(float InStamina)
{
	CurrentStamina = FMath::Clamp(InStamina, 0.0f, MaxStamina);
}

bool UCMovementComponent::HasEnoughStamina(float StaminaCost) const
{
	return CurrentStamina >= StaminaCost;
}

void UCMovementComponent::ReduceStamina(float StaminaCost)
{
	CurrentStamina -= StaminaCost;
}

void UCMovementComponent::DisableMovementInput()
{
	if (HitCounter == 0)
	{
		APlayerController* LocalPlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
		if (LocalPlayerController)
		{
			LocalPlayerController->SetIgnoreMoveInput(true);
		}
	}
	HitCounter++;
	GetWorld()->GetTimerManager().SetTimer(HitReactionTimerHandle, this, &UCMovementComponent::ResetHitCounter, 1.0f, false);
}

void UCMovementComponent::EnableMovementInput()
{
	APlayerController* LocalPlayerController = Cast<APlayerController>(OwnerCharacter->GetController());
	if (LocalPlayerController)
	{
		LocalPlayerController->SetIgnoreMoveInput(false);
	}
}

void UCMovementComponent::ResetHitCounter()
{
	HitCounter--;
	if (HitCounter <= 0)
	{
		HitCounter = 0;
		EnableMovementInput();
	}
}


void UCMovementComponent::OnDashing()
{
	if (CurrentStamina >= DashStaminaCost)
	{
		const FVector ForwardDir = OwnerCharacter->GetActorForwardVector().GetSafeNormal(); // ĳ������ ���� ����
		FVector StartLocation = OwnerCharacter->GetActorLocation();
		FVector EndLocation = StartLocation + ForwardDir * DashDistance; // ����� ��ġ ���

		// ���� Ʈ���̽��� ���� ��� �˻�
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerCharacter); // �ڱ� �ڽ��� �浹 �˻翡�� ����

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			CollisionParams
		);

		if (bHit)
		{
			// �浹 �������� ���
			EndLocation = HitResult.Location;
		}

		// ����� ��ġ�� ���� ����
		FVector DashDestination = FVector(EndLocation.X, EndLocation.Y, StartLocation.Z);

		OwnerCharacter->SetActorEnableCollision(false);
		OwnerCharacter->SetActorLocation(DashDestination, true); // ĳ������ ��ġ�� ����� ��ġ�� �̵�

		if (DashMontage)
		{
			OwnerCharacter->PlayAnimMontage(DashMontage, 1, NAME_None);
		}

		if (GhostTrailClass)
		{
			GhostTrail = CHelpers::Play_GhostTrail(GhostTrailClass, OwnerCharacter);
		}

		FTimerHandle DashCollisionTimerHandle;
		float DashDuration = 0.1f;

		GetWorld()->GetTimerManager().SetTimer(DashCollisionTimerHandle, this, &UCMovementComponent::EnableCollision, DashDuration, false);
		CurrentStamina -= DashStaminaCost;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough stamina to dash!"));
	}
}

void UCMovementComponent::OffDashing()
{
	if (!!GhostTrail)
		GhostTrail->Destroy();
}
