// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class U06_BATTLE_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPatrolPath();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	FVector GetMoveTo();
	void    UpdateIndex();

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }

private:
	UPROPERTY(EditAnywhere, Category = "Loop")
		bool bLoop;  // path를 level에 오려 놓고 수정

	UPROPERTY(EditAnywhere, Category = "Path")
		int32 Index;
	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;

private: //Component
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		class USplineComponent* Spline;
	UPROPERTY(VisibleAnywhere)
		class UTextRenderComponent* Text;


};
