// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "CAudioTrigger.generated.h"

UCLASS()
class U06_BATTLE_API ACAudioTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAudioTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
private:
	//UPROPERTY(VisibleAnywhere)
	//	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere)
		UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere)
		USoundCue* SoundCue;

	//UFUNCTION()
	//	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//
	//UFUNCTION()
	//	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
