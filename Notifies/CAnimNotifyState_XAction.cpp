// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_XAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CXAction.h"
#include "Weapons/SkillActions/CXAction_Sword.h"

FString UCAnimNotifyState_XAction::GetNotifyName_Implementation() const
{
	return "XAction";
}
void UCAnimNotifyState_XAction::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetXKeyAction());

	weapon->GetXKeyAction()->Begin_XAction();
}

void UCAnimNotifyState_XAction::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetXKeyAction());

	weapon->GetXKeyAction()->End_XAction();
}
