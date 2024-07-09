// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "Weapons/CWeaponstructures.h"
#include "CThornObject.generated.h"

UCLASS()
class U06_BATTLE_API ACThornObject : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
	
public:	
	ACThornObject();

protected:
	virtual void BeginPlay() override;

private: // Component
    UPROPERTY(VisibleDefaultsOnly)
        class USceneComponent* Root;
    UPROPERTY(VisibleDefaultsOnly)
        class UNiagaraComponent* Niagara;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Hit")
        FHitData HitData;

private:
    UPROPERTY(EditDefaultsOnly, Category = "Niagara")
        class UStaticMesh*  NiagaraMesh;
    UPROPERTY(EditDefaultsOnly, Category = "Niagara")
        FRotator  NiagaraMeshRotation;
private:
    UPROPERTY(EditDefaultsOnly, Category = "Niagara")
        class UFXSystemAsset*  CollisionEffect;
    UPROPERTY(EditDefaultsOnly, Category = "Niagara")
        FTransform  CollisionEffectTransform;


private:
    UFUNCTION()
        void OnSystemFinished(class UNiagaraComponent* PSyetem);

public:
    //INiagaraParticleCallbackHandler 의 함수구현
    //UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Niagara")
    //void ReceiveParticleData(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem);
    void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem) override;


private:
    FVector BoxExtent;

};
