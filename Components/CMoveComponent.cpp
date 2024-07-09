#include "Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
	: CurrentStamina(100.0f), // 초기 스태미나 설정
	MaxStamina(100.0f),    // 최대 스태미나 설정
	StaminaRecoveryRate(5.0f), // 스태미나 회복 속도 설정
	DashStaminaCost(20.0f)// 대시 당 스태미나 소모량
{
	CurrentPitch = 0.0f;
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
    // InGame에서 Parent를 가져 올수 있음 : 생성자(X)
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
	// 스태미나가 최대치를 초과하지 않도록 합니다.
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
		const FVector ForwardDir = OwnerCharacter->GetActorForwardVector().GetSafeNormal(); // 캐릭터의 전방 벡터
		FVector StartLocation = OwnerCharacter->GetActorLocation();
		FVector EndLocation = StartLocation + ForwardDir * DashDistance; // 대시할 위치 계산

		// 라인 트레이스를 통해 경로 검사
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(OwnerCharacter); // 자기 자신은 충돌 검사에서 제외

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			CollisionParams
		);

		if (bHit)
		{
			// 충돌 지점까지 대시
			EndLocation = HitResult.Location;
		}

		// 대시할 위치의 높이 보정
		FVector DashDestination = FVector(EndLocation.X, EndLocation.Y, StartLocation.Z);

		OwnerCharacter->SetActorEnableCollision(false);
		OwnerCharacter->SetActorLocation(DashDestination, true); // 캐릭터의 위치를 대시할 위치로 이동

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
