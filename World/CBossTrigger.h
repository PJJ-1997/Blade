// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossTrigger.generated.h"

class UBoxComponent;
class ACBossSpawner;

UCLASS()
class U06_BATTLE_API ACBossTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBossTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Trigger")
		UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class ACBossSpawner> BossSpawnerClass;

	ACBossSpawner* BossSpawner;

	bool bHasTriggered;
};
