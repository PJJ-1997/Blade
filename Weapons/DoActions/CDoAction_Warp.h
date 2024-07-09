// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Warp.generated.h"


UCLASS(Blueprintable)
class U06_BATTLE_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()
public:
    UCDoAction_Warp();


public: // Actor,AtctorCompont Like«œ∞‘
    virtual void BeginPlay
    (
        class ACharacter*   InOwner,
        class ACAttachment* InAttachment,
        class UCEquipment*  InEquipment,
        const TArray<FDoActionData>& InDoActionData,
        const TArray<FHitData>& InHitData
    ) override;
    virtual void Tick(float  InDeltaTime) override;  

public:
    void  DoAction() override;
    void  Begin_DoAction() override;

private:
    bool  GetCusrorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
private:
    class UDecalComponent* Decal;
    class APlayerController* PlayerController;
    FVector MoveToLocation = FVector::ZeroVector;
};
