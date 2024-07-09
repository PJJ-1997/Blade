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
	UPROPERTY(EditAnywhere)  // 카메라에 의해 충돌반지
		FVector SocketOffset = FVector(0, 30, 10);
	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag = false;  // 총/화살 : false
	UPROPERTY(EditAnywhere)  // 카메라에 의해 충돌반지
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
public: // 마우스
	virtual void Pressed()  override;
	virtual void Released()  override;

private: // Component
	class USpringArmComponent*   SpringArm; // Player사용된것을 보관
	class UCameraComponent*      Camera;    // Player사용된것을 보관

private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;   // X,Y,Z  LinearColor :4, Float : 1
	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData  AimData;
	UPROPERTY(EditAnywhere, Category = "Aiming")
		float  AimingSpeed = 200.0f; // TimeLine시 사용데이터

private: // Binding함수
	UFUNCTION()
		void OnAming(FVector Output); // FTimeline에 바인딩할 함수
private: // UTimelineComponent대신에 FTimeline : UObject에서는 사용 못함
	FTimeline  Timeline;
private:
	FAimData   OriginData;

private:
	float* Bend = nullptr;
};
