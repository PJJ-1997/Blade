// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "CAIController.generated.h"

UCLASS()
class U06_BATTLE_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();
protected:
	virtual void  BeginPlay()       override;
public:
	void StopAI();
private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;

protected:
	void   OnPossess(APawn* InPawn)  override;
	void   OnUnPossess()             override;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:   // ���ε�
	UFUNCTION()
		void  OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);


private: // �ܺ����� ������� ������ ����
	class ACEnemy_AI*           Enemy;
	class UAISenseConfig_Sight* Sight;
	class UCAIBehaviorComponent* Behavior;

};