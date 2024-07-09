// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Hammer.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCSubAction_Hammer : public UCSubAction
{
	GENERATED_BODY()


private:  // Hammer에서는 HitData대신 Aura를 이용
    UPROPERTY(EditDefaultsOnly, Category = "Aura")
        TSubclassOf<class ACAura> AuraClass;
    UPROPERTY(EditDefaultsOnly, Category = "Aura")
        FVector AuarLocation;


private:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
        FDoActionData ActionData;
//    UPROPERTY(EditDefaultsOnly, Category = "Action")
 //       FHitData	  HitData;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Add-On")
        TSubclassOf<class ACGhostTrail> GhostTrailClass;


public:
    virtual void Pressed() override;

public:
    virtual void Begin_SubAction_Implementation() override;
    virtual void End_SubAction_Implementation() override;


private:
    class ACGhostTrail* GhostTrail;
};
