// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CWeaponstructures.h"
#include "Characters/CEnemy.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProjectileHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestroyer);

UENUM(BlueprintType)
enum class EArrowType : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice")
};

UCLASS()
class U06_BATTLE_API ACArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	ACArrow();
private:
    UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
        float LifeSpanAfterCollision = 3.0f;

private:
    UPROPERTY(VisibleDefaultsOnly)
        class UCapsuleComponent* Capsule;
    UPROPERTY(VisibleDefaultsOnly)
        class UProjectileMovementComponent* Projectile;
	UPROPERTY(VisibleAnywhere)
		UNiagaraComponent* NiagaraComponent;

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	virtual void Tick(float DeltaTime) override;

private:
    UFUNCTION()
        void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
    void  Shoot(const FVector& InFoward);
	void  SetNiagaraEffect(UNiagaraSystem* NiagaraSystem);
	void  SetArrowType(EArrowType NewType);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Type", meta = (AllowPrivateAccess = "true"))
		EArrowType ArrowType;
	bool bHasBeenShot = false;
public:
    FORCEINLINE  void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:
    FProjectileHit       OnHit;
    FProjectileEndPlay   OnEndPlay;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UStaticMeshComponent* Static;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UNiagaraSystem* PathEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UNiagaraSystem* FireHitEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UNiagaraSystem* IceHitEffect;

private:
    TArray<AActor*> Ignores;

};