// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCZoomComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Zooming")
		float Speed = 20.0f;
	UPROPERTY(EditAnywhere, Category = "Zooming")
		float InterpSpeed = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Zooming")
		FVector2D Range = FVector2D(100, 500);

public:
	void  SetZoomValue(float InValue);

private:
	class USpringArmComponent* SpringArm;
	float CurrentValue;

};
