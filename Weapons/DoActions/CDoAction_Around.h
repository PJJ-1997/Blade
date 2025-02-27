// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Around.generated.h"


UCLASS(Blueprintable)
class U06_BATTLE_API UCDoAction_Around : public UCDoAction
{
	GENERATED_BODY()
	
private:
    UPROPERTY(EditAnywhere, Category = "SpawnClass")
        TArray < TSubclassOf<class ACRotate_Object>> RotateClasses;


public:
    void  DoAction() override;
    void  Begin_DoAction() override;

};
