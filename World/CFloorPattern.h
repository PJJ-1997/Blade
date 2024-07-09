// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h" 
#include "Weapons/CWeaponstructures.h"
#include "CFloorPattern.generated.h"

UCLASS()
class U06_BATTLE_API ACFloorPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	ACFloorPattern();

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Components")
		class UDecalComponent* Decal;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* CollisionBox; 

	UPROPERTY(EditAnywhere, Category = "Components")
		FHitData HitData;

	UFUNCTION()
		void OnTimerFinished();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void RemoveCollisionBox();
	void EnableCollisionBox();
};
