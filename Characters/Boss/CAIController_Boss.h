// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController_Boss.generated.h"

UCLASS()
class U06_BATTLE_API ACAIController_Boss : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController_Boss();
public:
	void StopAI();
	void StartAI();
protected:
	virtual void  BeginPlay()			override;
private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

protected:
	void   OnPossess(APawn* InPawn)  override;
	void   OnUnPossess()             override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:   // ¹ÙÀÎµù
	UFUNCTION()
		void  OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACBoss_AI*           Boss;
	class UAISenseConfig_Sight* Sight;
	class UCAIBehaviorComponent* Behavior;

private:
	UPROPERTY()
		class UBehaviorTree* BTBoss;
public:
	static const FName Target;
	static const FName Distance;
	static const FName Direction;
	static const FName SelectAttackNumber;
	static const FName SelectAttackNumber2;

public:
	UPROPERTY()
		float Radius;
};
