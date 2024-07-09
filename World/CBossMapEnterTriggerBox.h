// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Characters/CPlayer.h"
#include "CBossMapEnterTriggerBox.generated.h"

UCLASS()
class U06_BATTLE_API ACBossMapEnterTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ACBossMapEnterTriggerBox();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	void ShowNeedItemWidget();
	bool DoesPlayerHaveRequiredItem(AActor* PlayerActor);
	
private:
	UPROPERTY(EditAnywhere)
	FName LevelNameToLoad = "";
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> NeedItemWidgetClass;
};
