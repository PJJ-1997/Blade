// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_End_Parkour.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CParkourComponent.h"


FString UCAnimNotify_End_Parkour::GetNotifyName_Implementation() const
{
	return "End_Parkour";
}
void UCAnimNotify_End_Parkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(MeshComp->GetOwner());
	CheckNull(parkour);

	parkour->End_DoParkour();
}
