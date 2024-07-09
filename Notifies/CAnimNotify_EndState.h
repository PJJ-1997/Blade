// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/CStateComponent.h"
#include "CAnimNotify_EndState.generated.h"


UCLASS()
class U06_BATTLE_API UCAnimNotify_EndState : public UAnimNotify
{
	GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, Category = "Type")
        EStateType StateType;
public:
    FString GetNotifyName_Implementation() const override;
    void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
