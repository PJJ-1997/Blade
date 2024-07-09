// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CFeetComponent.h"
#include "CBossAnimInstance.generated.h"


UCLASS()
class U06_BATTLE_API UCBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UCBossAnimInstance();
	void PlayStartMontage();
	void PlayBackJump();
	//void PlayTurnLeft();
	//void PlayTurnRight();
	void PlayBasicAttack1();
	void PlayBasicAttack2();
	void PlayBasicAttack3();
	void PlayBasicAttack4();
	void PlayDashAttack();
	void PlayRangeAttack();
	void PlayDecalAttack();
	void PlayBlock();
	void PlayPowerUp();
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = true))
		UAnimMontage* StartMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* BackJump;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	//	UAnimMontage* TurnLeft;
	//
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	//	UAnimMontage* TurnRight;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* BasicAttack1;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* BasicAttack2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* BasicAttack3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* BasicAttack4;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* DashAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* RangeAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* DecalAttack;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* Block;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* PowerUp;
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
public:
	void PlayerToRotation(float DeltaSeconds);

};
