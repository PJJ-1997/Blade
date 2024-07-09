// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CWeaponData.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "CZAction.h"
#include "CXAction.h"
#include "CCAction.h"
#include "CDownAttack.h"
#include "Kismet/KismetMathLibrary.h" 

UCWeaponAsset::UCWeaponAsset()
{
    AttachmentClass = ACAttachment::StaticClass(); // BP_CAttacment_Sword
    EquipmentClass  = UCEquipment::StaticClass();
    DoActionClass   = UCDoAction::StaticClass();
    SubActionClass  = UCSubAction::StaticClass();
	ZActionClass	= UCZAction::StaticClass();
	XActionClass	= UCXAction::StaticClass();
	CActionClass	= UCCAction::StaticClass();
	DownAttackClass = UCDownAttack::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter * InOwner,class UCWeaponData** OutWeaponData)
{
    // 모두다 Local변수 --> UCWeaponData Class에 저장

    ACAttachment* Attachment = nullptr;
    if (!!AttachmentClass)
    {
        FActorSpawnParameters params;
        params.Owner = InOwner;

        Attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
    }

    UCEquipment* Equipment = nullptr;
    if (!!EquipmentClass)
    {
        // Object는 동적할당을 한다
        // 주의점) CEquipment.h에 정의된 OwnerCharacter은 개비지콜렉션으로 지워지지
        //         않기위해 시리얼라이즈를 해야 한다
        Equipment = NewObject<UCEquipment>(this, EquipmentClass);
        // 데이터 : EquipmentData
        //         몽타쥬데이터를 run이 실행
        //         Equip->NotiState(Begin_Equip) SpawnActor위치를 바꿔준다
        //                NotiState(Ebd_Equip)   SpawnActor위치를 바꿔준다
        // Attachment가 실제적인 형상이 들어간 BP가 되므로
        // BP_Attachment에 Noti를 넘겨준다
        Equipment->BeginPlay(InOwner, EquipmentData);

        if (!!Attachment) 
        {
            // BP상에서 바인딩 부분을 연결
            // BlueprintNativeEvent로 함수연결
            Equipment->OnEquipment_BeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
            Equipment->OnEquipment_EndEquip.AddDynamic(Attachment, &ACAttachment::OnEndEquip);
            Equipment->OnEquipment_UnEquip.AddDynamic(Attachment, &ACAttachment::OnUnEquip);
        }
    }

    UCDoAction* DoAction = nullptr;
    if (!!DoActionClass)
    {

        DoAction = NewObject<UCDoAction>(this, DoActionClass);
        DoAction->BeginPlay(InOwner, Attachment, Equipment,DoActionDatas,HitDatas);

        if (!!Attachment)
        {
            // BP상에서 바인딩 부분을 연결
            // BlueprintNativeEvent로 함수연결
            Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
            Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);

            Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
            Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);
        }

        if (!!Equipment)
        {
           Equipment->OnEquipment_BeginEquip.AddDynamic(DoAction, &UCDoAction::OnBeginEquip);
           Equipment->OnEquipment_UnEquip.AddDynamic(DoAction, &UCDoAction::OnUnEquip);

        }
    }

    // SubAction
    UCSubAction* SubAction = nullptr;
    if (!!SubActionClass)
    {
        SubAction = NewObject<UCSubAction>(this, SubActionClass);
        SubAction->BeginPlay(InOwner, Attachment, DoAction);

    }

    *OutWeaponData = NewObject<UCWeaponData>();
    (*OutWeaponData)->Attachment = Attachment;
    (*OutWeaponData)->SetAttachment(Attachment);
    (*OutWeaponData)->Equipment = Equipment;
    (*OutWeaponData)->SetEquipment(Equipment);
    (*OutWeaponData)->DoAction = DoAction;
    (*OutWeaponData)->SubAction = SubAction;

	// ZAction
	UCZAction* ZKeyAction = nullptr;
	if (!!ZActionClass)
	{
		ZKeyAction = NewObject<UCZAction>(this, ZActionClass);
		ZKeyAction->BeginPlay(InOwner, Attachment, DoAction);
	}

	UCXAction* XKeyAction = nullptr;
	if (!!XActionClass)
	{
		XKeyAction = NewObject<UCXAction>(this, XActionClass);
		XKeyAction->BeginPlay(InOwner, Attachment, DoAction);
	}

	UCDownAttack* QKeyAction = nullptr;
	if (!!DownAttackClass)
	{
		QKeyAction = NewObject<UCDownAttack>(this, DownAttackClass);
		QKeyAction->BeginPlay(InOwner, Attachment, DoAction);
	}

	UCCAction* CKeyAction = nullptr;
	if (!!CActionClass)
	{
		CKeyAction = NewObject<UCCAction>(this, CActionClass);
		CKeyAction->BeginPlay(InOwner, Attachment, DoAction);
	}

	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = Attachment;
	(*OutWeaponData)->SetAttachment(Attachment);
	(*OutWeaponData)->Equipment = Equipment;
	(*OutWeaponData)->SetEquipment(Equipment);
	(*OutWeaponData)->DoAction = DoAction;
	(*OutWeaponData)->SubAction  = SubAction;
	(*OutWeaponData)->ZKeyAction = ZKeyAction;
	(*OutWeaponData)->XKeyAction = XKeyAction;
	(*OutWeaponData)->CKeyAction = CKeyAction;
	(*OutWeaponData)->QKeyAction = QKeyAction;
}

UCEquipment * UCWeaponAsset::GetEquipment()
{
    return nullptr;
}

