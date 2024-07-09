// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CZAction.h"
#include "Weapons/CWeaponstructures.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CZAction_Sword.generated.h"


UCLASS(Blueprintable)
class U06_BATTLE_API UCZAction_Sword : public UCZAction
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
		TSubclassOf<class ACZAura> ZAuraClass;

	UPROPERTY(EditDefaultsOnly, Category = "Aura")
		FVector AuarLocation;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FDoActionData ActionData;
	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FHitData	  HitData;

public:
	virtual void Pressed() override;

public:
	virtual void Begin_ZAction_Implementation() override;
	virtual void End_ZAction_Implementation() override;
	virtual void Tick_Implementation(float InDeltaTime) override;


private:
	UFUNCTION()
		void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther);
	//   UFUNCTION()
	//       void OnAttachmentEndCollision();


private:    // 순간이동시 사용할 데이터
	bool    bMoving = false;

private:
	TArray<class ACharacter*> Hit;
	TArray<class ACharacter*> Overlapped;
	int32 HitIndex;

};
