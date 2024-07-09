// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossSpawner.generated.h"

class ACBoss_AI;

UCLASS()
class U06_BATTLE_API ACBossSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBossSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
		FVector BossScale;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ACBoss_AI> BossClass;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
		void SpawnBoss();
};
