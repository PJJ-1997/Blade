// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CBoss.h"
#include "Global.h"
#include "CGameMode.h"
#include "CAnimInstance.h"
#include "Characters/Boss/CBossAnimInstance.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Characters/Boss/CBoss_AI.h"
#include "Characters/CPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/BoxComponent.h"
#include "Widgets/CUserWidget_EndGame.h"
#include "Widgets/CUserWidget_Player.h"
#include "Widgets/CUserWidget_Cooldown.h"
#include "UserInterface/CSTutorialHUD.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/CWeaponstructures.h"
#include "Characters/Boss/CActor_RangeSword.h"
#include "NiagaraFunctionLibrary.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "TimerManager.h"

// Sets default values

ACBoss::ACBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");


	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/RavensS1/Meshes/Characters/Combines/SK_RavensS1_B.SK_RavensS1_B'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCBossAnimInstance> animInstance;
	CHelpers::GetClass<UCBossAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemies/Boss/ABP_Boss2.ABP_Boss2_C'");
	GetMesh()->SetAnimClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> START(TEXT("AnimMontage'/Game/Boss_BattleEntrance_RM_Montage.Boss_BattleEntrance_RM_Montage'"));
	if (START.Succeeded())
	{
		StartMontage = START.Object;
	}

	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxComponent"));
	HitBoxComponent->SetupAttachment(RootComponent);
	HitBoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACBoss::OnHitBoxOverlap);
	bIsBlocking = false;
	bHasAura = false;
	bBossRageMode = false;
	bIsAlive = true;
	AttackIndex = 0;
}

void ACBoss::BeginPlay()
{
	Super::BeginPlay();
	Create_DynamicMaterial(this);
	Change_Color(this, OriginColor);
	State->OnStateTypeChanged.AddDynamic(this, &ACBoss::OnStateTypeChanged);

	if (BossAI != nullptr)
	{
		BossAI->StopAI();
		BossAnimInstance = Cast<UCBossAnimInstance>(GetMesh()->GetAnimInstance());
		if (BossAnimInstance)
		{
			GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACBoss::AIStart, 11.0f, false);
		}
	}
	else
	{
		BossAI = Cast<ACAIController_Boss>(GetController());
		if (BossAI)
		{
			BossAI->StopAI();
		}
	}
	BossAICharacter = Cast<ACBoss_AI>(UGameplayStatics::GetActorOfClass(GetWorld(), ACBoss_AI::StaticClass()));
}
void ACBoss::Tick(float DeltaTime)
{
}

void ACBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BossAnimInstance = Cast<UCBossAnimInstance>(GetMesh()->GetAnimInstance());
	BossAI = Cast<ACAIController_Boss>(GetController());
	if (nullptr != BossAnimInstance)
	{
		BossAnimInstance->OnMontageEnded.AddDynamic(this, &ACBoss::OnAttackMontageEnded);
	}
}
void ACBoss::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking)
	{
		IsAttacking = false;
		OnAttackEnd.Broadcast();
	}
}

void ACBoss::AddAura()
{
	if (AuraNiagaraSystem)
	{
		Aura = UNiagaraFunctionLibrary::SpawnSystemAttached(
			AuraNiagaraSystem, 
			GetMesh(),         
			FName("pelvis"),
			FVector::ZeroVector,
			FRotator::ZeroRotator, 
			EAttachLocation::KeepRelativeOffset, 
			true              
		);

		if (Aura)
		{
			Aura->SetRelativeLocation(FVector::ZeroVector);
			Aura->SetRelativeRotation(FRotator::ZeroRotator);

			Aura->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f)); // 원하는 스케일 값으로 변경하세요.
		}
	}
}

void ACBoss::PlayTransformCinematic()
{
	if (BossTransformSequence)
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->bShowMouseCursor = false;
		}

		// 시네마틱 시작 시 매시 숨기기 및 Attach된 무기 숨기기
		SetActorHiddenInGame(true);
		HideAttachedActors();
		HidePlayerMesh();

		// BehaviorTree 멈추기
		if (BossAI)
		{
			BossAI->StopAI();
		}
		FMovieSceneSequencePlaybackSettings Settings;
		ALevelSequenceActor* SequenceActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			BossTransformSequence,
			Settings,
			SequenceActor
		);

		if (SequencePlayer)
		{
			SequencePlayer->Play();
			SetWidgetsVisibility(false); // 시네마틱 시작 시 위젯 숨기기
			SequencePlayer->OnFinished.AddDynamic(this, &ACBoss::OnTransformCinematicFinished);
		}
	}
}

void ACBoss::OnTransformCinematicFinished()
{   
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
	}

	// 시네마틱 종료 시 매시 보이기 및 Attach된 무기 보이기
	SetActorHiddenInGame(false);
	ShowAttachedActors();
	ShowPlayerMesh();

	// 일정 시간 후 BehaviorTree 재시작
	GetWorld()->GetTimerManager().SetTimer(RestartAITimerHandle, this, &ACBoss::DelayedAIStart, 15.0f, false);

	SetWidgetsVisibility(true); // 시네마틱 종료 시 위젯 보이기
}

void ACBoss::SetWidgetsVisibility(bool bVisible)
{
	if (ACPlayer* Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (Player->HPWidget)
		{
			Player->HPWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
		if (Player->CooldownWidget)
		{
			Player->CooldownWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		}
	}

	if (BossAICharacter)
	{
		if (bVisible)
		{
			BossAICharacter->ShowWidgets();
		}
		else
		{
			BossAICharacter->HideWidgets();
		}
	}
}

void ACBoss::HideAttachedActors()
{
	// Attach된 모든 액터 숨기기
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(true);
		}
	}
}

void ACBoss::ShowAttachedActors()
{
	// Attach된 모든 액터 보이기
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (AActor* Actor : AttachedActors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(false);
		}
	}
}

void ACBoss::HidePlayerMesh()
{
	if (ACPlayer* Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		Player->GetMesh()->SetVisibility(false);

		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		Player->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
		for (USkeletalMeshComponent* MeshComponent : SkeletalMeshComponents)
		{
			if (MeshComponent)
			{
				MeshComponent->SetVisibility(false);
			}
		}

		TArray<AActor*> AttachedActors;
		Player->GetAttachedActors(AttachedActors);
		for (AActor* Actor : AttachedActors)
		{
			if (Actor)
			{
				Actor->SetActorHiddenInGame(true);
			}
		}
	}
}

void ACBoss::ShowPlayerMesh()
{
	if (ACPlayer* Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		Player->GetMesh()->SetVisibility(true);

		TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
		Player->GetComponents<USkeletalMeshComponent>(SkeletalMeshComponents);
		for (USkeletalMeshComponent* MeshComponent : SkeletalMeshComponents)
		{
			if (MeshComponent)
			{
				MeshComponent->SetVisibility(true);
			}
		}

		TArray<AActor*> AttachedActors;
		Player->GetAttachedActors(AttachedActors);
		for (AActor* Actor : AttachedActors)
		{
			if (Actor)
			{
				Actor->SetActorHiddenInGame(false);
			}
		}
	}
}


bool ACBoss::IsInBossRageMode() const
{
	return bBossRageMode;
}

void ACBoss::SetAttackIndex(int32 InIndex)
{
	AttackIndex = InIndex;
	UE_LOG(LogTemp, Warning, TEXT("SetAttackIndex: %d"), AttackIndex); // 로그 추가
}

int32 ACBoss::GetAttackIndex() const
{
	return AttackIndex;
}

void ACBoss::PlayHitEffect(UNiagaraSystem * Effect, float Duration)
{
	if (Effect)
	{
		UNiagaraComponent* HitEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			Effect,
			GetMesh(),
			FName("pelvis"), // Pelvis 소켓에 붙임
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);

		if (HitEffect)
		{
			HitEffect->SetAutoDestroy(true);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				FTimerDelegate::CreateLambda([HitEffect]()
					{
						HitEffect->Deactivate();
					}),
				Duration,
						false
						);
		}
	}
}

void ACBoss::ApplyFireDamage(float DotDamage, float Duration)
{
	TotalDotDamage = DotDamage * Duration;
	RemainingDotTime = Duration;
	GetWorld()->GetTimerManager().SetTimer(DotDamageHandle, this, &ACBoss::DealDotDamage, 1.0f, true);
}

void ACBoss::ApplyIceEffect(float SlowDuration)
{
	GetCharacterMovement()->MaxWalkSpeed *= 0.5f; // 50% 감소
	GetWorld()->GetTimerManager().SetTimer(SlowHandle, this, &ACBoss::RemoveIceEffect, SlowDuration, false);
}

void ACBoss::DealDotDamage()
{
	if (RemainingDotTime <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(DotDamageHandle);
		return;
	}

	Status->Damage(TotalDotDamage / RemainingDotTime);
	RemainingDotTime--;

	if (Status->IsDead())
	{
		State->SetDeadMode();
		GetWorld()->GetTimerManager().ClearTimer(DotDamageHandle);
	}
}

void ACBoss::RemoveIceEffect()
{
	GetCharacterMovement()->MaxWalkSpeed /= 0.5f; // 원래 속도로 복구
}

void ACBoss::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hit:   Hit(); break;
	case EStateType::Dead:  Dead(); break;
	}
}


float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (bIsBlocking)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BlockSound, GetActorLocation());
		return 0.0f;
	}

	float CurrentHealth = Status->GetHealth();
	float MaxHealth = Status->GetMaxHealth();
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 현재 체력 출력
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f / %f"), CurrentHealth - damage, MaxHealth);

	if ((CurrentHealth - damage) / MaxHealth <= 0.5f && !bHasAura)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss health is below 50%%. Current Health: %f"), (CurrentHealth - damage) / MaxHealth);
		bBossRageMode = true;
		PlayTransformCinematic();
		AddAura();
		bHasAura = true;
	}

	Damage.power = damage;
	Damage.Character = Cast<ACharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHitMode();

	return damage;
}

void ACBoss::Hit()
{
	// Health 
	Status->Damage(Damage.power);
	Damage.power = 0.0f;

	{
		if (!!Damage.Event && !!Damage.Event->HitData)
		{
			// Montage
			Damage.Event->HitData->PlayBossMontage(this);
			// HitStop
			Damage.Event->HitData->PlayBossHitStop(GetWorld());
			// Sound
			Damage.Event->HitData->PlayBossSoundWave(this);
			// Effect
			Damage.Event->HitData->PlayBossEffect(GetWorld(), GetActorLocation(), GetActorRotation());



			if (Status->IsDead())
			{
				//   PrintLine();
				State->SetDeadMode();
				return;
			}
		}
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACBoss::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Montages->PlayDeadMode();

	// 보스 사망 후 위젯 보이게 하는 방법
	ACGameMode* GameMode = Cast<ACGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->OnBossDead();
	}
}

// Noti에서 당은 데이터
void ACBoss::End_Hit()
{
	State->SetIdleMode();
}

void ACBoss::End_Dead()
{
	if (Aura)
	{
		Aura->DestroyComponent();
		Aura = nullptr;  // 포인터를 nullptr로 설정
	}
	if (BossAI)
	{
		BossAI->StopAI();
	}
	bIsAlive = false;
}


void ACBoss::PlayStartMontage()
{
	if (BossAnimInstance && StartMontage)
	{
		BossAnimInstance->Montage_Play(StartMontage, 1.0f);
	}
}

void ACBoss::BackJump()
{
	if (!IsAttacking)
	{
		BossAnimInstance->PlayBackJump();
		IsAttacking = true;
	}
}
//void ACBoss::TurnLeft()
//{
//	if (!IsAttacking)
//	{
//		BossAnimInstance->PlayTurnLeft();
//		IsAttacking = true;
//	}
//}
//void ACBoss::TurnRight()
//{
//	if (!IsAttacking)
//	{
//		BossAnimInstance->PlayTurnRight();
//		IsAttacking = true;
//	}
//}

void ACBoss::AIStart()
{
	if (BossAI != nullptr)
		BossAI->StartAI();
}

void ACBoss::AIStop()
{
	if (BossAI != nullptr)
		BossAI->StopAI();
}

void ACBoss::DelayedAIStart()
{
	UE_LOG(LogTemp, Warning, TEXT("DelayedAIStart: Calling AIStart"));
	AIStart();
}

void ACBoss::BasicAttack1(AActor* OtherActor)
{
	SetAttackIndex(0); // BasicAttack1에 해당하는 인덱스 설정
	ACPlayer* Player = Cast<ACPlayer>(OtherActor);
	if (Player)
	{
		if (!IsAttacking)
		{
			BossAnimInstance->PlayBasicAttack1();
			IsAttacking = true;
		}
	}
}
void ACBoss::BasicAttack2()
{
	SetAttackIndex(1); // BasicAttack2에 해당하는 인덱스 설정
	if (!IsAttacking)
	{
		BossAnimInstance->PlayBasicAttack2();
		IsAttacking = true;
	}
}
void ACBoss::BasicAttack3()
{
	SetAttackIndex(2); // BasicAttack3에 해당하는 인덱스 설정
	if (!IsAttacking)
	{
		BossAnimInstance->PlayBasicAttack3();
		IsAttacking = true;
	}
}
void ACBoss::BasicAttack4()
{
	SetAttackIndex(3); // BasicAttack4에 해당하는 인덱스 설정
	if (!IsAttacking)
	{
		BossAnimInstance->PlayBasicAttack4();
		IsAttacking = true;
	}
	//if (!IsAttacking)
	//{
	//	BossAnimInstance->PlayBasicAttack4();
	//	IsAttacking = true;
	//}
}
void ACBoss::DashAttack()
{
	if (!IsAttacking)
	{
		BossAnimInstance->PlayDashAttack();
		IsAttacking = true;
	}
}
void ACBoss::RangeAttack()
{
	if (!IsAttacking)
	{
		BossAnimInstance->PlayRangeAttack();
		IsAttacking = true;
	}
}
void ACBoss::DecalAttack()
{
	if (!IsAttacking)
	{
		BossAnimInstance->PlayDecalAttack();
		IsAttacking = true;
	}
}
void ACBoss::Block()
{
	float BlockChance = 0.05f; // 5% 확률로 Block
	if (FMath::FRand() <= BlockChance && !bIsBlocking)
	{
		bIsBlocking = true;
		BossAnimInstance->PlayBlock();

		UE_LOG(LogTemp, Warning, TEXT("Blocking!"));
		GetWorldTimerManager().SetTimer(BlockTimerHandle, this, &ACBoss::EndBlock, 0.3f, false);
	}
}
void ACBoss::EndBlock()
{
	bIsBlocking = false;
	UE_LOG(LogTemp, Warning, TEXT("End Blocking"));
}

void ACBoss::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		Block();
	}
}