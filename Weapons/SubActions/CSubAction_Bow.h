// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT(BlueprintType)
struct FAimData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 100.0f;
	UPROPERTY(EditAnywhere)  // ī�޶� ���� �浹����
		FVector SocketOffset = FVector(0, 30, 10);
	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag = false;  // ��/ȭ�� : false
	UPROPERTY(EditAnywhere)  // ī�޶� ���� �浹����
		FVector CameraLocation = FVector(0, 0, 0);

};

UCLASS(Blueprintable)
class U06_BATTLE_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()

public:
	UCSubAction_Bow();
public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	virtual void Tick_Implementation(float InDeltaTime) override;
public: // ���콺
	virtual void Pressed()  override;
	virtual void Released()  override;

private: // Component
	class USpringArmComponent*   SpringArm; // Player���Ȱ��� ����
	class UCameraComponent*      Camera;    // Player���Ȱ��� ����

private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;   // X,Y,Z  LinearColor :4, Float : 1
	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData  AimData;
	UPROPERTY(EditAnywhere, Category = "Aiming")
		float  AimingSpeed = 200.0f; // TimeLine�� ��뵥����

private: // Binding�Լ�
	UFUNCTION()
		void OnAming(FVector Output); // FTimeline�� ���ε��� �Լ�
private: // UTimelineComponent��ſ� FTimeline : UObject������ ��� ����
	FTimeline  Timeline;
private:
	FAimData   OriginData;

private:
	float* Bend = nullptr;
};
