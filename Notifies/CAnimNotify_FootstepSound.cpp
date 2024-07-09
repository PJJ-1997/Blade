// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_FootstepSound.h"
#include "Global.h"
#include "Sound/SoundBase.h"

FString UCAnimNotify_FootstepSound::GetNotifyName_Implementation() const
{
	return "FootStep";
}
void UCAnimNotify_FootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && MeshComp->GetOwner() && SoundAsset)
	{
		FVector CharacterLocation = MeshComp->GetOwner()->GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(MeshComp->GetOwner(), SoundAsset, CharacterLocation, VolumeMultiplier);
	}
}