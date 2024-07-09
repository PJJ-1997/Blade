// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_CAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CCAction.h"

FString UCAnimNotifyState_CAction::GetNotifyName_Implementation() const
{
	return "CAction";
}
void UCAnimNotifyState_CAction::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// ABP�����ؼ��� InGame & ABP ������ �Ǳ� ������ NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCKeyAction());

	weapon->GetCKeyAction()->Begin_CAction();
}

void UCAnimNotifyState_CAction::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// ABP�����ؼ��� InGame & ABP ������ �Ǳ� ������ NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetCKeyAction());

	weapon->GetCKeyAction()->End_CAction();
}

