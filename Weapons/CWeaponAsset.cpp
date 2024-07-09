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
    // ��δ� Local���� --> UCWeaponData Class�� ����

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
        // Object�� �����Ҵ��� �Ѵ�
        // ������) CEquipment.h�� ���ǵ� OwnerCharacter�� �������ݷ������� ��������
        //         �ʱ����� �ø������� �ؾ� �Ѵ�
        Equipment = NewObject<UCEquipment>(this, EquipmentClass);
        // ������ : EquipmentData
        //         ��Ÿ�굥���͸� run�� ����
        //         Equip->NotiState(Begin_Equip) SpawnActor��ġ�� �ٲ��ش�
        //                NotiState(Ebd_Equip)   SpawnActor��ġ�� �ٲ��ش�
        // Attachment�� �������� ������ �� BP�� �ǹǷ�
        // BP_Attachment�� Noti�� �Ѱ��ش�
        Equipment->BeginPlay(InOwner, EquipmentData);

        if (!!Attachment) 
        {
            // BP�󿡼� ���ε� �κ��� ����
            // BlueprintNativeEvent�� �Լ�����
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
            // BP�󿡼� ���ε� �κ��� ����
            // BlueprintNativeEvent�� �Լ�����
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

