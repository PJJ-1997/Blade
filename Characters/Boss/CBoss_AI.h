// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CBoss.h"
#include "Characters/Boss/CAIController_Boss.h"
#include "Perception/AIPerceptionComponent.h"
#include "Widgets/CUserWidget_Label.h"
#include "CBoss_AI.generated.h"



UCLASS()
class U06_BATTLE_API ACBoss_AI : public ACBoss
{
	GENERATED_BODY()

public:
	ACBoss_AI();

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override; // 함수 선언 추가
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }

public:   // Enmey-->EnemyAI
	void Hit()  override;
	void Dead() override;

public:   // Interface에서 상속 받은 함수 oeveride
	void End_Hit()   override;
	void End_Dead()  override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		uint8  TeamID = 2;  // 255 == 중립, TeamID가 동일하면 아군

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class ACPatrolPath* PatrolPath;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCWeaponComponent* Weapon;
	UPROPERTY(VisibleDefaultsOnly)
		class UCAIBehaviorComponent* Behavior;

private:
	// CUserWidget_Stamina 클래스의 포인터 변수
	UCUserWidget_Label* BossHPWidget;
	ACAIController_Boss* BossAIController;
public:
	void UpdateHealthAndStaminaOnWidget(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintCallable, Category = "UI")
		void HideWidgets();

	UFUNCTION(BlueprintCallable, Category = "UI")
		void ShowWidgets();
};
