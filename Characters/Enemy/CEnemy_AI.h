// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Engine/DataTable.h"
#include "CEnemy_AI.generated.h"

class APickup;
class UItemBase;

UCLASS()
class U06_BATTLE_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()
public:
	ACEnemy_AI();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
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
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		float  LabelViewAmount = 3000.0f;   // 일정거리에서만 UI가 보일수 있게

#if WITH_EDITOR  // GameMode에서는 사용하지 않게
private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
		class  UWidgetComponent*  LabelWidget;
#endif

private:
	void UpdateLabelRenderScale();

protected:
	void DropItems();

	UPROPERTY(EditAnywhere, Category = "Drop")
		UDataTable* ItemDataTable; // 드롭될 아이템의 데이터 테이블
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
		TSubclassOf<APickup> DropItemClass;
	UPROPERTY(EditAnywhere, Category = "Drop")
		FName DesiredItemID; // 드롭될 아이템의 ID
	UPROPERTY(EditAnywhere, Category = "Drop")
		int32 DropItemQuantity;
};

