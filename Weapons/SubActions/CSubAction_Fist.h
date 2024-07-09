// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Fist.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U06_BATTLE_API UCSubAction_Fist : public UCSubAction
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = "Action")
        FDoActionData ActionData;
    UPROPERTY(EditAnywhere, Category = "Action")
        TArray<FHitData> HitDatas;
    UPROPERTY(EditAnywhere, Category = "Add-On")
        TSubclassOf<class ACGhostTrail> GhostTrailClass; 


public:
    virtual void Begin_SubAction_Implementation() override;
    virtual void End_SubAction_Implementation() override;
public:
    virtual void Pressed() override;



private:
    UFUNCTION()
        void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther);
    UFUNCTION()
        void OnAttachmentEndCollision();

private:
    TArray<class ACharacter*> Hit;
    int32 HitIndex;
    class ACGhostTrail* GhostTrail;
	
};
