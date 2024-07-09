// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_ZAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CZAction.h"

FString UCAnimNotifyState_ZAction::GetNotifyName_Implementation() const
{
	return "ZAction";
}
void UCAnimNotifyState_ZAction::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetZKeyAction());

	weapon->GetZKeyAction()->Begin_ZAction();
}

void UCAnimNotifyState_ZAction::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetZKeyAction());

	weapon->GetZKeyAction()->End_ZAction();
}
