// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

// 콘트롤러로 BB을 이용하여 :  콘트롤러 빙의시 setting
// BB에 Setting, Get할 함수를 가지고 있는 Class

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Dead, Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U06_BATTLE_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAIBehaviorComponent();
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AIStateTypeKey = "AIState";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName TargetKey = "Target";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName PatrolLocationKey = "Patrol_Location";
	UPROPERTY(EditAnywhere, Category = "Key")
		FName AvoidLocationKey = "Avoid_Location";

public:
	class   ACharacter*  GetTarget();
public:    //패트롤
	FVector GetPatrolLocation();
	void    SetPatrolLocation(const FVector& InLocation);
public:  //Avoid
	FVector GetAvoidLocation();

public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsAvoidMode();
	bool IsDeadMode();


public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	FAIStateTypeChanged  OnAIStateTypeChanged;

private:
	EAIStateType GetType();
	void         ChangeType(EAIStateType InType);

private:
	class UBlackboardComponent*  Blackboard;  // AIController 빙의시 변수 세팅


};
