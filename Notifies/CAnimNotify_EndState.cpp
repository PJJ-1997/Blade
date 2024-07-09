// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Characters/ICharacter.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
    return "EndState";
}
void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    Super::Notify(MeshComp, Animation);

    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());

    IICharacter* interface = Cast<IICharacter>(MeshComp->GetOwner());
    CheckNull(interface);

    switch (StateType)
    {
    case EStateType::BackStep: interface->End_Avoid(); break;
    case EStateType::Roll:     interface->End_Avoid(); break;
    case EStateType::Hit:      interface->End_Hit(); break;
    case EStateType::Dead:     interface->End_Dead(); break;

    }
	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	if (weapon)
	{
		if (weapon->GetDoAction())
		{
			weapon->GetDoAction()->End_DoAction();
		}
	}

}
