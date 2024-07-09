// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponstructures.h"
#include "CRotate_Object.generated.h"

UCLASS()
class U06_BATTLE_API ACRotate_Object : public AActor
{
	GENERATED_BODY()
	
public:	
	ACRotate_Object();

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;


private:  // Around할 파티클
    UPROPERTY(VisibleAnywhere)
        class UCapsuleComponent* Capsule;
    UPROPERTY(VisibleAnywhere)
        class UParticleSystemComponent* Particle;


private:
    UFUNCTION()
        void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION()
        void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    UFUNCTION()
        void SendMessage();


private:
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
        FHitData HitData;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        float Speed = 300.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        float Distance = 150.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        bool  bNegative;
    UPROPERTY(EditDefaultsOnly, Category = "Spawn")
        float DamageInterval = 0.1f;


private: 
    float  Angle = 0.0f;
    FTimerHandle  TimerHandle; // SendMessage()을 Timer로 바인딩시 핸들
    TArray<ACharacter*> Hit;


};
