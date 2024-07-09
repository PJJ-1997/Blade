// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CWeaponstructures.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CSubAction_Sword.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
        float Distance = 1000.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Trace")
        float Speed = 20.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Trace") // DebugLine
        TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Action")
        FDoActionData ActionData;
    UPROPERTY(EditDefaultsOnly, Category = "Action")
        FHitData	  HitData;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Add-On")
        TSubclassOf<class ACGhostTrail> GhostTrailClass;

public:
    virtual void Pressed() override;

public:
    virtual void Begin_SubAction_Implementation() override;
    virtual void End_SubAction_Implementation() override;
    virtual void Tick_Implementation(float InDeltaTime) override;


private:
  UFUNCTION()
       void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther);
 //   UFUNCTION()
 //       void OnAttachmentEndCollision();


private:    // 순간이동시 사용할 데이터
    bool    bMoving = false;
    FVector Start;
    FVector End;


private:
    TArray<class ACharacter*> Hit;
    TArray<class ACharacter*> Overlapped;
    int32 HitIndex;
    class ACGhostTrail* GhostTrail;
};
