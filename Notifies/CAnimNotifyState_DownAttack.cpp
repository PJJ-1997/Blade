// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_DownAttack.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDownAttack.h"

FString UCAnimNotifyState_DownAttack::GetNotifyName_Implementation() const
{
	return "DownAttack";
}
void UCAnimNotifyState_DownAttack::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	// ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetQKeyAction());

	weapon->GetQKeyAction()->Begin_DownAttack();
}

void UCAnimNotifyState_DownAttack::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	// ABP관련해서는 InGame & ABP 동작이 되기 때문에 NullCheck
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetQKeyAction());

	weapon->GetQKeyAction()->End_DownAttack();
}

