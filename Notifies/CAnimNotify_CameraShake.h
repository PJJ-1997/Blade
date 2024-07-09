// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()
	
private:
    UPROPERTY(EditAnywhere, Category = "CameraShake")
        TSubclassOf<class UMatineeCameraShake> CameraShakeClass;


public:
    FString GetNotifyName_Implementation() const override;
    void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
