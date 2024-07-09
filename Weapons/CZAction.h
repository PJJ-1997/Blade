// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponstructures.h"
#include "CZAction.generated.h"

UCLASS(Blueprintable)
class U06_BATTLE_API UCZAction : public UObject
{
	GENERATED_BODY()
public:
	UCZAction();
public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);

public: // 마우스
	virtual void Pressed();
	virtual void Released();

public:
	UFUNCTION(BlueprintNativeEvent)
		void Begin_ZAction();
	virtual void Begin_ZAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void End_ZAction();
	virtual void End_ZAction_Implementation() {};

	UFUNCTION(BlueprintNativeEvent)
		void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {};

protected:
	bool    bInAction = false; // action중인지 check flag

	UPROPERTY()
		class   ACharacter*             Owner;
	UPROPERTY()
		class   ACAttachment*           Attachment;
	UPROPERTY()
		class   UCDoAction*             DoAction;
	UPROPERTY()
		class   UCStateComponent*       State;
	UPROPERTY()
		class   UCMovementComponent*    Movement;

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
};
