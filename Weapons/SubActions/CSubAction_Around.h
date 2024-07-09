// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponstructures.h"
#include "CSubAction_Around.generated.h"


UCLASS()
class U06_BATTLE_API UCSubAction_Around : public UCSubAction
{
	GENERATED_BODY()
	
public:
    UCSubAction_Around();

private:
    UPROPERTY(EditDefaultsOnly, Category = "FallObject")
        TSubclassOf<class ACThornObject> ObjectClass;

    UPROPERTY(EditDefaultsOnly, Category = "FallObject")
        FVector ObjectLocation;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
        FDoActionData ActionData;



public: // ¸¶¿ì½º
    virtual void Pressed()    override;
    virtual void Begin_SubAction_Implementation() override;
    virtual void End_SubAction_Implementation() override;


};
