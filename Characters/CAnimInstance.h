// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"


UCLASS()
class U06_BATTLE_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    void NativeBeginPlay() override;
    void NativeUpdateAnimation(float DeltaSeconds) override;

protected: // EditAnywhere : AnimGraph에서 사용가능하게끔 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Pitch;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        bool bFalling;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        EWeaponType WeaponType = EWeaponType::Max;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bow")
		bool bBow_Aiming;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
		bool bFeet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InverseKinemetics")
		FFeetData FeetData;

private:
    UFUNCTION()
        void  OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
private:
    class ACharacter*        OwnerCharacter;
    class UCWeaponComponent* Weapon;

private:
    FRotator  PrevRotation;  // BP와 interp용으로 

};
