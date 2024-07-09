// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_FootstepSound.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCAnimNotify_FootstepSound : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundAsset;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	//	FName SocketName = FName("FootSocket"); // 블루프린트에서 지정할 소켓 이름
};
