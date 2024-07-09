// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
    Walk=0, Run, Sprint,Max,
};

// BP에서 컴포넌트가 추가하지 않게 하려면
UCLASS(ClassGroup = (Custom))
//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMovementComponent();
protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "CameraSpeed")
        float HorizontalLook = 45.0f;
    UPROPERTY(EditAnywhere, Category = "CameraSpeed")
        float VerticalLook = 45.0f;
	UPROPERTY(EditAnywhere, Category = "CameraAngle")
		float MinPitchAngle = -80.0f;
	UPROPERTY(EditAnywhere, Category = "CameraAngle")
		float MaxPitchAngle = 80.0f;
	UPROPERTY(EditAnywhere, Category = "CameraAngle")
		float CurrentPitch;
private:
    UPROPERTY(EditAnywhere, Category = "Speed")
        float Speed[(int32)ESpeedType::Max] = { 200,400,600 };

private:
	float DashDistance = 1000;

private:
	UPROPERTY(EditAnywhere)
		UAnimMontage* DashMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Add-On")
		TSubclassOf<class ACGhostTrail> GhostTrailClass;

private:
	class ACGhostTrail* GhostTrail;

public:
    FORCEINLINE bool CanMove() { return bCanMove; }
    FORCEINLINE void Move() { bCanMove = true; }
    FORCEINLINE void Stop() { bCanMove = false; }

    FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
    FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
    FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

    FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
    FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
    FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

    FORCEINLINE void EnableTopViewCamera()  { bTopViewCamera = true; }
    FORCEINLINE void DisableTopViewCamera() { bTopViewCamera = false; }



public:
    void OnMoveForward(float InAxis);
    void OnMoveRight(float InAxis);
    void OnHorizontalLook(float InAxis);
    void OnVerticalLook(float InAxis);

public:
    void EnableControlRotation();
    void DisableControlRotation();

public:
    void OnWalk();
    void OnRun();
    void OnSprint();
	void OnDashing();
	void OffDashing();
    void SetSpeed(ESpeedType InType);

public:
	// 캐릭터가 공중에 있는지 확인하는 함수
	bool IsFalling() const;

private:
    bool bCanMove = true;
    bool bFixedCamera = false;
    bool bTopViewCamera = false;
public:
	float MaxStamina; // 최대 스태미나
	float CurrentStamina; // 현재 스태미나
	float DashStaminaCost; // 대시 동작에 소비되는 스태미나
	float StaminaRecoveryRate;
	float HammerStaminaCost = 20.0f; // 원하는 양으로 조정하세요
public:
	void RecoverStamina(float DeltaTime);
	void EnableCollision();
private:
    class ACharacter* OwnerCharacter;  // 각 콤포넌트마다 생성시 변수를 저장
public:
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return CurrentStamina; }
public:
	void SetStamina(float InStamina);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool HasEnoughStamina(float StaminaCost) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void ReduceStamina(float StaminaCost);

	void DisableMovementInput();
	void EnableMovementInput();
	void ResetHitCounter();

private:
	FTimerHandle HitReactionTimerHandle;
	int32 HitCounter;
};
