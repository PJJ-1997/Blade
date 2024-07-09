// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Boss/CBossAnimInstance.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CFeetComponent.h"
#include "Weapons/CSubAction.h"

UCBossAnimInstance::UCBossAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> START(TEXT("AnimMontage'/Game/Boss_BattleEntrance_RM_Montage.Boss_BattleEntrance_RM_Montage'"));
	if (START.Succeeded())
	{
		StartMontage = START.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BACKJUMP(TEXT("AnimMontage'/Game/BackStep_Montage.BackStep_Montage'"));
	if (BACKJUMP.Succeeded())
	{
		BackJump = BACKJUMP.Object;
	}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> TURNLEFT(TEXT("AnimMontage'/Game/Turn_Combat_180_L_Seq_Montage.Turn_Combat_180_L_Seq_Montage'"));
	//if (TURNLEFT.Succeeded())
	//{
	//	TurnLeft = TURNLEFT.Object;
	//}
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> TURNRIGHT(TEXT("AnimMontage'/Game/Turn_Combat_180_R_Seq_Montage.Turn_Combat_180_R_Seq_Montage'"));
	//if (TURNRIGHT.Succeeded())
	//{
	//	TurnRight = TURNRIGHT.Object;
	//}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BASICATTACK1(TEXT("AnimMontage'/Game/Enemies/Animation/Montages/NewBoss/Boss_Attack_Running_Simple_RM_Montage.Boss_Attack_Running_Simple_RM_Montage'"));
	if (BASICATTACK1.Succeeded())
	{
		BasicAttack1 = BASICATTACK1.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BASICATTACK2(TEXT("AnimMontage'/Game/Enemies/Animation/Montages/NewBoss/Boss_Attack_SwingAndSlam_RM_Montage.Boss_Attack_SwingAndSlam_RM_Montage'"));
	if (BASICATTACK2.Succeeded())
	{
		BasicAttack2 = BASICATTACK2.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BASICATTACK3(TEXT("AnimMontage'/Game/Enemies/Animation/Montages/NewBoss/Boss_Attack_Uppercut_RM_Montage.Boss_Attack_Uppercut_RM_Montage'"));
	if (BASICATTACK3.Succeeded())
	{
		BasicAttack3 = BASICATTACK3.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BASICATTACK4(TEXT("AnimMontage'/Game/Frank_RPG_Katana_S1_Combo01_02_Montage.Frank_RPG_Katana_S1_Combo01_02_Montage'"));
	if (BASICATTACK4.Succeeded())
	{
		BasicAttack4 = BASICATTACK4.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DASHATTACK(TEXT("AnimMontage'/Game/Frank_RPG_Assassin_Jump_01_Montage.Frank_RPG_Assassin_Jump_01_Montage'"));
	if (DASHATTACK.Succeeded())
	{
		DashAttack = DASHATTACK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RANGEATTACK(TEXT("AnimMontage'/Game/Attack_Up_01_Seq_Montage.Attack_Up_01_Seq_Montage'"));
	if (RANGEATTACK.Succeeded())
	{
		RangeAttack = RANGEATTACK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DECALATTACK(TEXT("AnimMontage'/Game/Enemies/Animation/Animations/NewBoss/Boss_Idle_Montage.Boss_Idle_Montage'"));
	if (DECALATTACK.Succeeded())
	{
		DecalAttack = DECALATTACK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BLOCK(TEXT("AnimMontage'/Game/Frank_RPG_GreatSword_Block_Montage.Frank_RPG_GreatSword_Block_Montage'"));
	if (BLOCK.Succeeded())
	{
		Block = BLOCK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> POWERUP(TEXT("AnimMontage'/Game/Boss_Staggered_RM_Montage.Boss_Staggered_RM_Montage'"));
	if (POWERUP.Succeeded())
	{
		PowerUp = POWERUP.Object;
	}
}

void UCBossAnimInstance::PlayStartMontage()
{
	Montage_Play(StartMontage, 1.0f);
}

void UCBossAnimInstance::PlayBackJump()
{
	Montage_Play(BackJump, 1.0f);
}

//void UCBossAnimInstance::PlayTurnLeft()
//{
//	Montage_Play(TurnLeft, 1.0f);
//}
//void UCBossAnimInstance::PlayTurnRight()
//{
//	Montage_Play(TurnRight, 1.0f);
//}
void UCBossAnimInstance::PlayBasicAttack1()
{
	Montage_Play(BasicAttack1, 1.0f);
}

void UCBossAnimInstance::PlayBasicAttack2()
{
	Montage_Play(BasicAttack2, 1.0f);
}

void UCBossAnimInstance::PlayBasicAttack3()
{
	Montage_Play(BasicAttack3, 1.0f);
}

void UCBossAnimInstance::PlayBasicAttack4()
{
	Montage_Play(BasicAttack4, 1.0f);
}
void UCBossAnimInstance::PlayDashAttack()
{
	Montage_Play(DashAttack, 1.0f);
}
void UCBossAnimInstance::PlayRangeAttack()
{
	Montage_Play(RangeAttack, 1.0f);
}
void UCBossAnimInstance::PlayDecalAttack()
{
	Montage_Play(DecalAttack, 1.0f);
}
void UCBossAnimInstance::PlayBlock()
{
	Montage_Play(Block, 1.0f);
}
void UCBossAnimInstance::PlayPowerUp()
{
	Montage_Play(PowerUp, 1.0f);
}
void UCBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	// 바인딩하기위해
	// 
	// DECLARE_EVENT(UCAnimInstance, test);                       // Class내에서만
	// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(Ftest,int32,A);  --> NineParam
	// 틀린점 : OnWeaponTypeChanged.Broadcast(prevType, InType);
	//         --> Broadcast를 어디에서든지 Call할수 있는게 DECLARE_DYNAMIC_MULTICAST_DELEGATE
	//         --> DECLARE_EVENT는 Class내에서만 BroadCast를 Call가능
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);

	if (!!Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCBossAnimInstance::OnWeaponTypeChanged);
		//   EWeaponType prevType = EWeaponType::Bow;
		//   EWeaponType inType = EWeaponType::Bow;
		//   if (Weapon->OnWeaponTypeChanged.IsBound())
		//       Weapon->OnWeaponTypeChanged.Broadcast(prevType, inType);
	}


}
void UCBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	// 이동 속도 계산
	Speed = OwnerCharacter->GetVelocity().Size2D();

	// 방향 계산
	FRotator rotatorCharacter = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotatorControl = OwnerCharacter->GetActorRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotatorCharacter, rotatorControl);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	// 피치 조정
	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	// 떨어지는 상태 업데이트
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	// 무기 상태 업데이트
	CheckNull(Weapon);
	bBow_Aiming = true;
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming &= (WeaponType == EWeaponType::Bow);
		bBow_Aiming &= (Weapon->GetSubAction()->GetInAction());
	}

	// 파쿠르 및 발 상태 업데이트
	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);
	UCFeetComponent* feet = CHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);

	bFeet = false;
	if (!!parkour && !!feet)
	{
		bFeet = parkour->IsExecuting() ? false : true;
		FeetData = feet->GetData();
	}
	else if (!!feet)
	{
		bFeet = true;
		FeetData = feet->GetData();
	}
///////////////////////////////////////////////////////////////////////// ---> BTService에서 처리
	// 데드 모드 일때는 활성화 x
	PlayerToRotation(DeltaSeconds);

	//플레이어를 바라보는 로직 추가
	//ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//if (PlayerCharacter)
	//{
	//	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
	//	FRotator ControlRotation = FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaSeconds, 5.0f); // 회전 속도 조절
	//	OwnerCharacter->SetActorRotation(ControlRotation);
	//}
}



void UCBossAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCBossAnimInstance::PlayerToRotation(float DeltaSeconds)
{
	ACBoss* Boss = Cast<ACBoss>(OwnerCharacter);
	if (Boss && Boss->bIsAlive)
	{
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (PlayerCharacter)
		{
			// 보스와 플레이어 간의 거리 계산
			float DistanceToPlayer = FVector::Dist(OwnerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());

			// 거리 기반 회전 각도 제한
			float MaxRotationSpeed = FMath::Clamp(5.0f * DistanceToPlayer / 100.0f, 0.5f, 5.0f);

			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
			FRotator ControlRotation = FMath::RInterpTo(OwnerCharacter->GetActorRotation(), TargetRotation, DeltaSeconds, MaxRotationSpeed);

			// Pitch 제한
			ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch, 0.0f, 0.0f);  // 예시로 Pitch를 -10도에서 10도 사이로 제한 // -> 0도로 두어서 pitch(기울기) 값 0

			OwnerCharacter->SetActorRotation(ControlRotation);
		}
	}
}


