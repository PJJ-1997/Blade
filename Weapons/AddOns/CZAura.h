// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponstructures.h"
#include "NiagaraDataInterfaceExport.h"
#include "CZAura.generated.h"

UCLASS()
class U06_BATTLE_API ACZAura : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

public:
	ACZAura();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnSystemFinished(class UNiagaraComponent* PSyetem);

public:
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem) override;


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

private: // Hit���� ������
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		FHitData HitData;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		float DamageInterval = 0.1f;

private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere)
		class UNiagaraComponent* Niagara;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* Projectile;
private:
	TArray<class ACharacter*> Hit;
	FTimerHandle TimerHandle;

};
