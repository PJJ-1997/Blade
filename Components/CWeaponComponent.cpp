
#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "Characters/CBoss.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CZAction.h"
#include "Weapons/CXAction.h"
#include "Weapons/CCAction.h"
#include "Weapons/CDownAttack.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	AttackRange = 200.0f; // ���� ���� ����
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    // InGame���� Parent�� ���� �ü� ���� : ������(X)
    // int A[3];
    // �迭�� �̸� �ּ���
    // A[0]�� �ּ� --> &A[0]
    OwnerCharacter = Cast<ACharacter>(GetOwner());
    for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
    {
        if (!!DataAssets[i])
            DataAssets[i]->BeginPlay(OwnerCharacter,&Datas[i]); // Asset�� �ִ� �����͸� �ʱ�ȭ
    }

}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!!GetDoAction())
        GetDoAction()->Tick(DeltaTime);

    if (!!GetSubAction())
        GetSubAction()->Tick(DeltaTime);


}

void UCWeaponComponent::SetUnarmedMode()
{
    CheckFalse(IsIdleMode());
    GetEquipment()->UnEquip();
    ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
    // idle�� �ƴϸ� �������� �ȵ�
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Sword);
  
}

void UCWeaponComponent::SetHammerMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetAroundMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Around);
}

void UCWeaponComponent::SetBowMode()
{
    CheckFalse(IsIdleMode());
    SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::DoAction()
{
    if (!!GetDoAction())
        GetDoAction()->DoAction();

	PerformAttack();
}


bool UCWeaponComponent::IsIdleMode()
{
    return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{

    if (Type == InType) // ���� Key�� 2���̻� ���� ���(����/��������)
    {
        SetUnarmedMode();  // 2(����������),2(Unarmd)
        return;
    }
    if (IsUnarmedMode() == false)
    {
        // TODO
        // Equipment�� ��������
        GetEquipment()->UnEquip();
    }
    
    Datas[(int32)InType]->GetEquipment()->Equip();

    ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
    EWeaponType prevType = Type;
    Type = InType;
    if (OnWeaponTypeChanged.IsBound())
        OnWeaponTypeChanged.Broadcast(prevType, InType);
}

ACAttachment * UCWeaponComponent::GetAttachment()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetAttachment();
}

UCEquipment * UCWeaponComponent::GetEquipment()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);

    return Datas[(int32)Type]->GetEquipment();
}

UCDoAction * UCWeaponComponent::GetDoAction()
{

    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);
    return Datas[(int32)Type]->GetDoAction();
}

UCSubAction*  UCWeaponComponent::GetSubAction()
{
    CheckTrueResult(IsUnarmedMode(), nullptr);
    CheckFalseResult(!!Datas[(int32)Type], nullptr);
    return Datas[(int32)Type]->GetSubAction();
}
UCZAction * UCWeaponComponent::GetZKeyAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);
	return Datas[(int32)Type]->GetZKeyAction();
}
UCXAction * UCWeaponComponent::GetXKeyAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);
	return Datas[(int32)Type]->GetXKeyAction();
}
UCCAction * UCWeaponComponent::GetCKeyAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);
	return Datas[(int32)Type]->GetCKeyAction();
}
UCDownAttack * UCWeaponComponent::GetQKeyAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);
	return Datas[(int32)Type]->GetQKeyAction();
}

void UCWeaponComponent::PerformAttack()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector Start = Owner->GetActorLocation();
	FVector End = Start + (Owner->GetActorForwardVector() * AttackRange);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Owner);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn, CollisionParams);

	if (bHit)
	{
		ACBoss* HitBoss = Cast<ACBoss>(HitResult.GetActor());
		if (HitBoss)
		{
			if (HitBoss->bIsBlocking)
			{
				// Block ������ �� �ٸ� �Ҹ� ���
				//UGameplayStatics::PlaySoundAtLocation(this, HitBoss->BlockSound, HitResult.ImpactPoint);
				UE_LOG(LogTemp, Warning, TEXT("Attack Blocked!"));
				return; // �������� ��ȿȭ�ϰ� ��ȯ
			}
			else
			{
				// Block �õ�
				HitBoss->Block();
			}
		}
	}

	// �ð��� ������� ���� ���� Ʈ���̽� �׸���
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);
}

void UCWeaponComponent::SubAction_Pressed()
{
  
    if (!!GetSubAction())
        GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
    if (!!GetSubAction())
        GetSubAction()->Released();
}

void UCWeaponComponent::ZKeyAction_Pressed()
{
	if (!!GetZKeyAction())
		GetZKeyAction()->Pressed();
}

void UCWeaponComponent::ZKeyAction_Released()
{
	if (!!GetZKeyAction())
		GetZKeyAction()->Released();
}

void UCWeaponComponent::XKeyAction_Pressed()
{
	if (!!GetXKeyAction())
		GetXKeyAction()->Pressed();
}

void UCWeaponComponent::XKeyAction_Released()
{
	if (!!GetXKeyAction())
		GetXKeyAction()->Released();
}

void UCWeaponComponent::CKeyAction_Pressed()
{
	if (!!GetCKeyAction())
		GetCKeyAction()->Pressed();
}

void UCWeaponComponent::CKeyAction_Released()
{
	if (!!GetCKeyAction())
		GetCKeyAction()->Released();
}

void UCWeaponComponent::QKeyAction_Pressed()
{
	if (!!GetQKeyAction())
		GetQKeyAction()->Pressed();
}

void UCWeaponComponent::QKeyAction_Released()
{
	if (!!GetQKeyAction())
		GetQKeyAction()->Released();
}

void UCWeaponComponent::DestoryWeapon()
{
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!Datas[i] && Datas[i]->GetAttachment())
			Datas[i]->GetAttachment()->Destroy();
	}
}