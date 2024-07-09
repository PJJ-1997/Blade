#include "Characters/CPlayer.h"
#include "Global.h"
#include "CGameInstance.h"
#include "Characters/CAnimInstance.h"
#include "Characters/CEnemy.h"
#include "Characters/CBoss.h"
#include "Characters/Enemy/CEnemy_AI.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/CPlayerController.h"
#include "Characters/Enemy/CAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapons/SkillActions/CCAction_Sword.h"
#include "Weapons/SkillActions/CZAction_Hammer.h"
#include "Widgets/CUserWidget_FKey.h"
#include "Widgets/CUserWidget_Label.h"
#include "Widgets/CUserWidget_Player.h"
#include "Widgets/CUserWidget_EndGame.h"
#include "Widgets/CUserWidget_Cooldown.h"
#include "UserInterface/CSTutorialHUD.h"
#include "World/Pickup.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
    // Create Components
    CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());

	// Camera
    CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	Camera->bUsePawnControlRotation = true;

    // Set Mesh & Anim Instance
    {
        GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
        GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

        USkeletalMesh* mesh;
        CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/ArashiAkuma/Meshes/Characters/Combines/SK_Arashi_E.SK_Arashi_E'");
        GetMesh()->SetSkeletalMesh(mesh);

        TSubclassOf<UCAnimInstance> animInstance;
        CHelpers::GetClass<UCAnimInstance>(&animInstance, "AnimBlueprint'/Game/Player/ABP_Player.ABP_Player_C'");
        GetMesh()->SetAnimClass(animInstance);

		SpringArm->SetRelativeLocation(FVector(0, 0, 140));
		SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
		SpringArm->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
		SpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
		SpringArm->bDoCollisionTest = false;
    }

    // ActorComponent
    {
        CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
        CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
        CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
        CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
		CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
		CHelpers::CreateActorComponent<UCFeetComponent>(this, &Feet, "Feet");
		CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
		CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
    }
	CreateParkourArrow();
	
#if WITH_EDITOR
	{
		CHelpers::CreateComponent<UWidgetComponent>(this, &FKeyWidget, "FKey", GetMesh());

		TSubclassOf<UCUserWidget_FKey> fkeyClass;
		CHelpers::GetClass<UCUserWidget_FKey>(&fkeyClass, "WidgetBlueprint'/Game/Widgets/WB_FKey.WB_FKey_C'");

		FKeyWidget->SetWidgetClass(fkeyClass);
		FKeyWidget->SetRelativeLocation(FVector(0, 0, 220));
		FKeyWidget->SetDrawSize(FVector2D(120, 0));
		FKeyWidget->SetWidgetSpace(EWidgetSpace::Screen);
		
	}
#endif

	static ConstructorHelpers::FObjectFinder<USoundBase> LandingSoundAsset(TEXT("SoundWave'/Game/Assets154/Assets154/Content/Ultimate_SFX/Survival_Sound_Kit_HD_Remake/Wavs/Footsteps_Grass__Running__8.Footsteps_Grass__Running__8'"));
	if (LandingSoundAsset.Succeeded())
	{
		LandingSound = LandingSoundAsset.Object;
	}

	// Block
	bIsBlocking = false;
	BlockAnimationInterval = 0.7f;

	// Inventory
	{
		InteractionCheckFrequency = 0.1f;
		InteractionCheckDistance = 225.0f;
		BaseEyeHeight = 74.0f;

		PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
		PlayerInventory->SetSlotsCapacity(20);
		PlayerInventory->SetWeightCapacity(50.0f);

		AimingCameraTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimingCameraTimeline"));
		DefaultCameraLocation = FVector{ 0.0f,0.0f,65.0f };
		AimingCameraLocation = FVector{ 175.0f,50.0f,55.0f };
		SpringArm->SocketOffset = DefaultCameraLocation;
	}
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (!GameInstance->bIsPlayerDataInitialized)
		{
			TArray<UItemBase*> DefaultInventory;
			GameInstance->InitializePlayerData(Status->GetMaxHealth(), Movement->GetMaxStamina(), DefaultInventory);
		}
		LoadPlayerData();
	}
    Movement->SetSpeed(ESpeedType::Run);
    Movement->DisableControlRotation();
    // Biding In Game에서 해주어야한다
    State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	PlayerController = Cast<ACPlayerController>(GetController());

#if WITH_EDITOR
	FKeyWidget->InitWidget();
	UCUserWidget_FKey* fkey = Cast<UCUserWidget_FKey>(FKeyWidget->GetUserWidgetObject());
	fkey->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
	fkey->UpdateName(GetName());
	fkey->UpdateControllerName(GetController()->GetName());
#endif
	// Player Health Widget
	// 처음 시작할 시 위젯 삭제
	// HUD로 넘기기
	FString CurrentMapName = GetWorld()->GetMapName();

	if (CurrentMapName.Contains(TEXT("Start")))
	{
		return;
	}

	UClass* WidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, TEXT("WidgetBlueprint'/Game/Widgets/WB_Label_Player2.WB_Label_Player2_C'"));
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	
	if (Widget)
	{
		Widget->AddToViewport();
		HPWidget = Cast<UCUserWidget_Player>(Widget);
	}
	if (HPWidget)
	{
		UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth(), Movement->GetStamina(), Movement->GetMaxStamina());
	}
	// Cooldown
	if (CooldownWidgetClass)
	{
		CooldownWidget = CreateWidget<UCUserWidget_Cooldown>(GetWorld(), CooldownWidgetClass);
		if (CooldownWidget)
		{
			CooldownWidget->AddToViewport();
		}
		GetWorld()->GetTimerManager().SetTimer(CooldownUpdateTimerHandle, this, &ACPlayer::UpdateCooldowns, 0.1f, true);
	}
	// Item Widget
	{
		HUD = Cast<ACSTutorialHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	}
	// Inventory Aim
	{
		FOnTimelineFloat AimLerpAlphaValue;
		FOnTimelineEvent TimelineFinishedEvent;
		AimLerpAlphaValue.BindUFunction(this, FName("UpdateCameraTimeline"));
		TimelineFinishedEvent.BindUFunction(this, FName("CameraTimelineEnd"));

		if (AimingCameraTimeline && AimingCameraCurve)
		{
			AimingCameraTimeline->AddInterpFloat(AimingCameraCurve, AimLerpAlphaValue);
			AimingCameraTimeline->SetTimelineFinishedFunc(TimelineFinishedEvent);
		}
	}
	if (Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &ACPlayer::OnWeaponTypeChanged);
	}
}


void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Weapon->IsUnarmedMode())
	{
		UCUserWidget_FKey* fkey = Cast<UCUserWidget_FKey>(FKeyWidget->GetUserWidgetObject());
		if (!!fkey)
		{
			if (IsSkillOnCooldown("FKey"))
			{
				fkey->SetVisibility(ESlateVisibility::Collapsed);
				return;
			}

			// 플레이어와 가장 가까운 적을 찾습니다.
			ACEnemy_AI* nearestEnemy = nullptr;
			float closestDistance = FLT_MAX;

			TArray<AActor*> allEnemies;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy_AI::StaticClass(), allEnemies);

			for (AActor* enemyActor : allEnemies)
			{
				ACEnemy_AI* enemyAI = Cast<ACEnemy_AI>(enemyActor);
				if (enemyAI)
				{
					float distance = FVector::Distance(GetActorLocation(), enemyAI->GetActorLocation());
					if (distance < closestDistance)
					{
						closestDistance = distance;
						nearestEnemy = enemyAI;
					}
				}
			}

			if (nearestEnemy)
			{
				FVector PlayerToEnemy = (nearestEnemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				FVector EnemyForward = nearestEnemy->GetActorForwardVector();
				float DotProduct = FVector::DotProduct(PlayerToEnemy, EnemyForward);

				// 가장 가까운 적의 거리를 확인하고, 200 이하일 때만 위젯을 표시
				// 적의 등 뒤에 있을 때만 위젯을 표시 (DotProduct > -0.5f)
				if (closestDistance <= 200.0f && DotProduct > -0.5f)
				{
					fkey->SetVisibility(ESlateVisibility::Visible);
				}
				else
				{
					fkey->SetVisibility(ESlateVisibility::Collapsed);
				}

				// 디버그 메시지로 상태 확인
				//UE_LOG(LogTemp, Warning, TEXT("Closest distance: %f"), closestDistance);
				//UE_LOG(LogTemp, Warning, TEXT("DotProduct: %f"), DotProduct);
			}
			else
			{
				fkey->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	//Widget
	if (HPWidget)
	{
		UpdateHealthAndStaminaOnWidget(Status->GetHealth(), Status->GetMaxHealth(), Movement->GetStamina(), Movement->GetMaxStamina());
	}
	if (Movement)
	{
		Movement->RecoverStamina(DeltaTime);
	}
	// Interaction
	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}

	// Camera
	FVector StartLocation = SpringArm->GetComponentLocation();
	FVector EndLocation = Camera->GetComponentLocation();
	FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	float TargetArmLength = MaxCameraArmLength;

	FHitResult OutHit;
	FVector TraceEnd = StartLocation + (Direction * MaxCameraArmLength);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = false;
	float HitDistance = MaxCameraArmLength;

	// Perform multiple shorter collision checks along the path
	const int NumSteps = 5;
	for (int i = 1; i <= NumSteps; ++i)
	{
		FVector IntermediateEnd = StartLocation + (Direction * (MaxCameraArmLength * (i / (float)NumSteps)));
		if (GetWorld()->SweepSingleByChannel(OutHit, StartLocation, IntermediateEnd, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(ProbeSize), CollisionParams))
		{
			bHit = true;
			HitDistance = OutHit.Distance;
			break;
		}
	}

	if (bHit)
	{
		TargetArmLength = FMath::Clamp(HitDistance, 5.0f, MaxCameraArmLength);
	}

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, 10.0f);

}

void ACPlayer::UpdateHealthAndStaminaOnWidget(float CurrentHealth, float MaxHealth, float CurrentStamina, float MaxStamina)
{
	// HPWidget이 유효한지 확인
	if (HPWidget)
	{
		// Health 및 Stamina 업데이트 함수 호출
		HPWidget->UpdateHealth(Status->GetHealth(), Status->GetMaxHealth());
		HPWidget->UpdateStamina(Movement->GetStamina(), Movement->GetMaxStamina());
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement Binging : 함수포인터형이기 때문에 Serialixe하지 않는다
    PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
    PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
    PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
    PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

    // Avoid & Sprint
    PlayerInputComponent->BindAction("Avoid",  EInputEvent::IE_Released, this, &ACPlayer::OnAvoid);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
    PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);
	//PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnDashing);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &ACPlayer::OnEKeyPressed);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Released, Movement, &UCMovementComponent::OffDashing);

	//Zoom
	PlayerInputComponent->BindAxis("Zoom",this,&ACPlayer::SetZooming);
    // 무기 바인딩
    {
        PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
        PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
        PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
        PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);
        PlayerInputComponent->BindAction("Around", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAroundMode);
        PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);

    }

    // Action 바인딩
    PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

    // SubAction 바인딩
    PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnRightButton);
    PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffRightButton);
	// Item
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Pressed, this, &ACPlayer::BeginInteract);
	PlayerInputComponent->BindAction("Interact", EInputEvent::IE_Released, this, &ACPlayer::EndInteract);
	//Pause
	PlayerInputComponent->BindAction("TogglePauseMenu", EInputEvent::IE_Pressed, this, &ACPlayer::TogglePauseMenu);
	//PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::Aim);
	//PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::StopAiming);
	PlayerInputComponent->BindAction("ToggleMenu", EInputEvent::IE_Pressed, this, &ACPlayer::ToggleMenu);
	// SKill
	PlayerInputComponent->BindAction("ZAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnZKeyPressed);
	PlayerInputComponent->BindAction("ZAction", EInputEvent::IE_Released, this, &ACPlayer::OffZKeyPressed);
	PlayerInputComponent->BindAction("XAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnXKeyPressed);
	PlayerInputComponent->BindAction("XAction", EInputEvent::IE_Released, this, &ACPlayer::OffXKeyPressed);
	PlayerInputComponent->BindAction("CAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnCKeyPressed);
	PlayerInputComponent->BindAction("CAction", EInputEvent::IE_Released, this, &ACPlayer::OffCKeyPressed);
	PlayerInputComponent->BindAction("DownAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnQKeyPressed);
	PlayerInputComponent->BindAction("DownAttack", EInputEvent::IE_Released, this, &ACPlayer::OffQKeyPressed);
	PlayerInputComponent->BindAction("Assassination", EInputEvent::IE_Pressed, this, &ACPlayer::PerformStealthKill);
	//PlayerInputComponent->BindAction("Assassination", EInputEvent::IE_Released, this, &ACPlayer::OffStealthKill);

	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &ACPlayer::OnBlock);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &ACPlayer::OffBlock);
}
void ACPlayer::Landed(const FHitResult & Hit)
{
	Parkour->DoParkour(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandingSound, GetActorLocation());
}
void ACPlayer::SetGenericTeamId(const FGenericTeamId & NewTeamID)
{
	if (TeamID != NewTeamID)
		TeamID = NewTeamID;
}
float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (bIsBlocking)
	{
		// 캐릭터가 블록 중인 경우 블록 방향을 기준으로 한 각도를 계산하여 블록 범위 내에 있는지 확인
		FVector BlockDirection = GetActorForwardVector();
		FVector DamageDirection = DamageCauser->GetActorLocation() - GetActorLocation();
		DamageDirection.Normalize();

		float DotProduct = FVector::DotProduct(BlockDirection, DamageDirection);
		float DamageAngle = FMath::Acos(DotProduct) * (180.0f / PI);

		// 블록 각도보다 작거나 같으면 블록 범위 내에 있으므로 효과음만 재생하고 데미지를 받지 않음
		float DesiredBlockAngle = 45.0f;
		if (DamageAngle <= DesiredBlockAngle)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockingSound, GetActorLocation());
			return 0.0f; // 데미지를 0으로 반환하여 받지 않음
		}
	}

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 데미지 값을 Damage.power에 할당
	Damage.power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHitMode();

	// 이동 입력을 무시하도록 설정
	Movement->DisableMovementInput();

	ACBoss* Boss = Cast<ACBoss>(DamageCauser);
	if (Boss && Boss->IsInBossRageMode())
	{
		DamageAmount *= 1.5f; // RageMode일 때 데미지 증가
		UE_LOG(LogTemp, Warning, TEXT("DamageAmount: %f"), DamageAmount);
	}

	return damage;
}
void  ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
    switch (InNewType)
    {
	case EStateType::Idle:				   break;
	case EStateType::Equip:				   break;
	case EStateType::Dead:	   Dead();	   break;
	case EStateType::Action:			   break;
	case EStateType::Hit:	   Hitted();   break;
	case EStateType::BackStep: BackStep(); break;
	case EStateType::Roll:     Roll();     break;
    default:
         break;

    }
    
}

void ACPlayer::OnWeaponTypeChanged(EWeaponType PrevType, EWeaponType NewType)
{
	if (HPWidget)
	{
		// 모든 이미지를 초기화 (비활성화)
		HPWidget->HighlightSwordImage(false);
		HPWidget->HighlightHammerImage(false);
		HPWidget->HighlightBowImage(false);

		// 새로운 무기 유형만 활성화
		switch (NewType)
		{
		case EWeaponType::Sword:
			HPWidget->HighlightSwordImage(true);
			break;
		case EWeaponType::Hammer:
			HPWidget->HighlightHammerImage(true);
			break;
		case EWeaponType::Bow:
			HPWidget->HighlightBowImage(true);
			break;
		default:
			break;
		}
	}
}

void ACPlayer::ShowWidgets()
{
	if (HPWidget)
	{
		HPWidget->SetVisibility(ESlateVisibility::Visible);
	}
	if (CooldownWidget)
	{
		CooldownWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACPlayer::HideWidgets()
{
	if (HPWidget)
	{
		HPWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (CooldownWidget)
	{
		CooldownWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void  ACPlayer::OnAvoid()
{
	if (IsSkillOnCooldown("Space")) return;

	// idle이고 moving가능하면 회피동작
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	// 스태미나가 충분한지 확인
	if (!Movement->HasEnoughStamina(AvoidStaminaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough stamina to avoid!"));
		return;
	}

	float value = InputComponent->GetAxisValue("MoveForward");
	if (value > 0.0f)
	{
		State->SetRollMode();
		Roll();
	}
	else
	{
		State->SetBackStepMode();
		BackStep();
	}

	// 쿨타임 설정 및 스태미나 감소
	SetSkillCooldown("Space", 1.0f);
	Movement->ReduceStamina(AvoidStaminaCost);
}

void  ACPlayer::BackStep()
{
    Movement->EnableControlRotation();
    // 몽타주 play
    Montages->PlayBackStepMode();
}

void  ACPlayer::Roll()
{
    Movement->EnableControlRotation();
    Montages->PlayRollMode();
}

void ACPlayer::Hitted()
{
	// Health 
	Status->Damage(Damage.power);
	Damage.power = 0.0f;


	if (!!Damage.Event && !!Damage.Event->HitData)
	{
		// Montage
		Damage.Event->HitData->PlayMontage(this);
		// HitStop
		Damage.Event->HitData->PlayHitStop(GetWorld());
		// Sound
		Damage.Event->HitData->PlaySoundWave(this);
		// Effect
		Damage.Event->HitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		if (Status->IsDead())
		{
			State->SetDeadMode();
			return;
		}
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}
void ACPlayer::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Montages->PlayDeadMode();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		ACSTutorialHUD* TutorialHUD = Cast<ACSTutorialHUD>(PC->GetHUD());
		if (TutorialHUD)
		{
			TutorialHUD->ShowEndGameWidget();
		}
	}

	// AI를 멈추는 로직 추가
	ACBoss_AI* Boss = Cast<ACBoss_AI>(UGameplayStatics::GetActorOfClass(GetWorld(), ACBoss_AI::StaticClass()));
	if (Boss)
	{
		ACAIController_Boss* BossAIController = Cast<ACAIController_Boss>(Boss->GetController());
		if (BossAIController)
		{
			BossAIController->StopAI();
			UE_LOG(LogTemp, Warning, TEXT("Boss AI stopped because player died"));
		}
	}

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy_AI::StaticClass(), Enemies);

	for (AActor* EnemyActor : Enemies)
	{
		ACEnemy_AI* Enemy = Cast<ACEnemy_AI>(EnemyActor);
		if (Enemy)
		{
			ACAIController* EnemyAIController = Cast<ACAIController>(Enemy->GetController());
			if (EnemyAIController)
			{
				EnemyAIController->StopAI();
				UE_LOG(LogTemp, Warning, TEXT("Enemy AI stopped because player died"));
			}
		}
	}
}

void ACPlayer::End_Avoid()
{
    Movement->DisableControlRotation();
    State->SetIdleMode();
}

void ACPlayer::End_Hit()
{
	State->SetIdleMode();
	Movement->EnableMovementInput();
}

void ACPlayer::End_Dead()
{

}

void ACPlayer::OnRightButton()
{
	if (Weapon->IsBowMode())
	{
		Weapon->SubAction_Pressed();
	}

	if (Weapon->IsUnarmedMode())
	{
		Parkour->DoParkour();
		return;
	}

	if (IsSkillOnCooldown("RightButton"))
		return;

	if (Weapon->IsSwordMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 4.0f);
	}
	if (Weapon->IsHammerMode())
	{
		Weapon->SubAction_Pressed();
		SetSkillCooldown("RightButton", 5.0f);
	}
}

void ACPlayer::OffRightButton()
{
    // Bow 모드인 경우
    Weapon->SubAction_Released();
}

void ACPlayer::OnZKeyPressed()
{
	if (IsSkillOnCooldown("ZKey"))
		return;

	if (Weapon->IsSwordMode())
	{
		Weapon->ZKeyAction_Pressed();
		SetSkillCooldown("ZKey", 3.0f); // 쿨타임 10초 설정
	}

	if (Weapon->IsHammerMode())
	{
		bZKeyHeld = true;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ACPlayer::RepeatZKeyAction, 0.5f, true);
		RepeatZKeyAction();
		SetSkillCooldown("ZKey", 5.0f); // 쿨타임 10초 설정
	}
	if (Weapon->IsBowMode())
	{
		Weapon->ZKeyAction_Pressed();
		SetSkillCooldown("ZKey", 1.0f); // 쿨타임 10초 설정
	}
}

void ACPlayer::OffZKeyPressed()
{
	bZKeyHeld = false;
	GetWorldTimerManager().ClearTimer(TimerHandle);

	if (Weapon->IsHammerMode()) {
		Weapon->ZKeyAction_Released();
	}
}

void ACPlayer::RepeatZKeyAction()
{
	// Z 키가 눌리고 무기가 해머 모드인 경우
	if (bZKeyHeld && Weapon->IsHammerMode()) {
		
		UCMovementComponent* MovementComponent = Cast<UCMovementComponent>(Movement);

		if (MovementComponent && MovementComponent->CurrentStamina >= MovementComponent->HammerStaminaCost) {
			Weapon->ZKeyAction_Pressed();

			MovementComponent->CurrentStamina -= MovementComponent->HammerStaminaCost;
		}
		else 
		{
			GetWorldTimerManager().ClearTimer(TimerHandle);
		}
	}
	else {
		// Z 키가 해제되었거나 해머 모드가 아닐 경우 타이머 해제
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}

}


void ACPlayer::OnXKeyPressed()
{
	if (IsSkillOnCooldown("XKey")) return;

	if (Weapon->IsSwordMode())
	{
		Weapon->XKeyAction_Pressed();
		SetSkillCooldown("XKey", 8.0f); // 쿨타임 10초 설정
	}
}

void ACPlayer::OffXKeyPressed()
{
	Weapon->XKeyAction_Released();
}

void ACPlayer::OnQKeyPressed()
{
	if (IsSkillOnCooldown("QKey")) return;

	if (GetCharacterMovement()->IsFalling() && Weapon->IsSwordMode())
	{
		Weapon->QKeyAction_Pressed();
		SetSkillCooldown("QKey", 8.0f);
	}
}

void ACPlayer::OffQKeyPressed()
{
	Weapon->QKeyAction_Released();
}

void ACPlayer::OnCKeyPressed()
{
	if (IsSkillOnCooldown("CKey")) return;

	if (Weapon->IsSwordMode())
	{
		Weapon->CKeyAction_Pressed();
		SetSkillCooldown("CKey", 5.0f);
	}
}

void ACPlayer::OffCKeyPressed()
{
	Weapon->CKeyAction_Released();
}

void ACPlayer::StealthKill()
{
	if (Weapon->IsUnarmedMode())
	{
		PrintLine();
		PlayAnimMontage(StealthMontage, 1.0, NAME_None);
		SetSkillCooldown("FKey", 4.0f);

		FKeyWidget->SetVisibility(false);

		FTimerHandle VisibilityTimerHandle;
		// 일정 시간 후에 위젯을 다시 보이게 함
		GetWorld()->GetTimerManager().SetTimer(
			VisibilityTimerHandle,
			[this]()
			{
				if (!IsSkillOnCooldown("FKey"))
				{
					FKeyWidget->SetVisibility(true);
				}
			},
			4.5f,
				false
				);
	}
}

void ACPlayer::PerformStealthKill()
{
	if (Weapon->IsUnarmedMode())
	{
		if (IsSkillOnCooldown("FKey"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown"));
			return;
		}

		FHitResult HitResult;
		FVector Start = GetActorLocation();
		FVector End = Start + GetActorForwardVector() * 100.0f; // Example range

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams);
		if (bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Line trace hit something"));

			if (HitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit actor class: %s"), *HitResult.GetActor()->GetClass()->GetName());
			}

			ACEnemy* Target = Cast<ACEnemy>(HitResult.GetActor());
			if (Target)
			{
				FVector PlayerToEnemy = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
				FVector EnemyForward = Target->GetActorForwardVector();

				float DotProduct = FVector::DotProduct(PlayerToEnemy, EnemyForward);

				// 적의 등 뒤에 있는지 체크 (DotProduct가 0보다 작으면 등 뒤에 있는 것)
				if (DotProduct > -0.5f)
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit actor is an ACEnemy and player is behind the enemy"));
					StealthKill();
					Target->StealthDead();
				}
			}
		}
	
	// 디버그 라인 그리기 (옵션)
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
	}
}

void ACPlayer::OnEKeyPressed()
{
	if (IsSkillOnCooldown("EKey")) return;

	Movement->OnDashing();
	SetSkillCooldown("EKey", 4.0f);
}

void ACPlayer::SetZooming(float InValue)
{
	CheckTrue(Weapon->IsBowMode());
	Zoom->SetZoomValue(InValue);
}

void ACPlayer::CreateParkourArrow()
{
	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "ArrowGroup", GetCapsuleComponent());

	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		// Enum의 요소의 index 번째의 name을 FString으로 가져온다.
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name), ArrowGroup);

		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			break;
		case EParkourArrowType::Ceil:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;
		case EParkourArrowType::Floor:
			Arrows[i]->ArrowColor = FColor::Blue; 
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;
		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
			break;
		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, 30, 0));
			break; 
		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;
		}
	}
}

void ACPlayer::OnBlock()
{
	if (!bIsBlocking)
	{
		StartBlock();
	}
}

void ACPlayer::OffBlock()
{
	if (bIsBlocking)
	{
		StopBlock();
	}
}

void ACPlayer::StartBlock()
{
	bIsBlocking = true;
	PlayBlockAnimation();
}

void ACPlayer::StopBlock()
{
	bIsBlocking = false;
}

void ACPlayer::PlayBlockAnimation()
{
	if (Weapon->IsSwordMode() || Weapon->IsHammerMode())
	{
		if (Montages)
		{
			Montages->PlayBlockAnimation();
		}
	}
}
//void ACPlayer::OnHitReactionEnd()
//{
//	Movement->Activate();
//}
//////////////////////////////////////////////////////////////////////
// Inventory
//////////////////////////////////////////////////////////////////////
void ACPlayer::ToggleMenu()
{
	HUD->ToggleMenu();
}
void ACPlayer::TogglePauseMenu()
{
	HUD->TogglePauseMenu();
}
void ACPlayer::Aim()
{
	if (State->IsIdleMode())
	{
		if (!HUD->bIsMenuVisible)
		{
			bAiming = true;
			bUseControllerRotationYaw = true;
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
			if (AimingCameraTimeline)
			{
				AimingCameraTimeline->PlayFromStart();
			}
		}
	}
}

void ACPlayer::StopAiming()
{
	if (bAiming)
	{
		bAiming = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		if (AimingCameraTimeline)
		{
			AimingCameraTimeline->Reverse();
		}
	}
}

void ACPlayer::UpdateCameraTimeline(const float TimelineValue) const
{
	const FVector CameraLocation = FMath::Lerp(DefaultCameraLocation, AimingCameraLocation, TimelineValue);
	SpringArm->SocketOffset = CameraLocation;
}

void ACPlayer::CameraTimelineEnd()
{
	if (AimingCameraTimeline)
	{
		if (AimingCameraTimeline->GetPlaybackPosition() != 0.0f)
		{
			//HUD -> DisplayCrosshair();
		}
	}
}

void ACPlayer::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{ GetPawnViewLocation() };
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance) };

	double  LookDirection = FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector());

	if (LookDirection > 0)
	{
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
				if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				{
					return;
				}
			}
		}
	}
	NoInteractableFound();
}

void ACPlayer::FoundInteractable(AActor * NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}
	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();

}

void ACPlayer::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ACPlayer::BeginInteract()
{
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&ACPlayer::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void ACPlayer::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ACPlayer::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->Interact(this);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Pickupound, GetActorLocation());
	}
}


void ACPlayer::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}
// Item Drop
void ACPlayer::DropItem(UItemBase * ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);
		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DropSound, GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

void ACPlayer::SavePlayerData()
{
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SavePlayerData(Status->GetHealth(), Movement->GetStamina(), PlayerInventory->GetInventoryContents());
	}
}

void ACPlayer::LoadPlayerData()
{
	UCGameInstance* GameInstance = Cast<UCGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		float Health, Stamina;
		TArray<UItemBase*> Inventory;
		GameInstance->LoadPlayerData(Health, Stamina, Inventory);
		Status->SetHealth(Health);
		Movement->SetStamina(Stamina);

		PlayerInventory->GetInventoryContents().Empty();

		for (UItemBase* Item : Inventory)
		{
			PlayerInventory->HandleAddItem(Item);
		}
	}

}

void ACPlayer::SetSkillCooldown(FName SkillName, float CooldownTime)
{
	SkillCooldowns.Add(SkillName, CooldownTime);
	FTimerHandle& CooldownTimer = CooldownTimers.FindOrAdd(SkillName);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, [this, SkillName]() { ResetCooldown(SkillName); }, CooldownTime, false);
	if (CooldownWidget)
	{
		CooldownWidget->ShowCooldown(SkillName);
		CooldownWidget->UpdateCooldown(SkillName, CooldownTime);
	}
}

bool ACPlayer::IsSkillOnCooldown(FName SkillName) const
{
	return SkillCooldowns.Contains(SkillName);
}

void ACPlayer::ResetCooldown(FName SkillName)
{
    SkillCooldowns.Remove(SkillName);
    CooldownTimers.Remove(SkillName);
    if (CooldownWidget)
    {
        CooldownWidget->HideCooldown(SkillName);
    }
}

void ACPlayer::UpdateCooldowns()
{
	TArray<FName> Keys;
	SkillCooldowns.GetKeys(Keys);

	for (FName Key : Keys)
	{
		if (SkillCooldowns[Key] > 0.0f)
		{
			SkillCooldowns[Key] -= 0.1f;
			if (CooldownWidget)
			{
				CooldownWidget->UpdateCooldown(Key, SkillCooldowns[Key]);
			}
		}
		else
		{
			ResetCooldown(Key);
		}
	}
}