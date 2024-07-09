// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_SpawnDecal.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCBTService_SpawnDecal : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_SpawnDecal();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void SpawnActors(UBehaviorTreeComponent& OwnerComp);

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		int32 NumberOfActorsToSpawn = 20;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		float SpawnRadius = 500.0f;

	FTimerHandle TimerHandle;
};
