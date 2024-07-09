// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_End_BowString.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCAnimNotify_End_BowString : public UAnimNotify
{
    GENERATED_BODY()

public:
    FString GetNotifyName_Implementation() const override;
    void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
