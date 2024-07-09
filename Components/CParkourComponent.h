// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
#include "CParkourComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0, Ceil, Floor, Left, Right, Land, Max
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0, Fall, Slide, Short, Normal, Wall, Max
};

// 파쿠르데이터 테이블
USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

		// 기존에는 BP에서 행으로 구분 --> TMAP으로 가능
public:
	UPROPERTY(EditAnywhere) // Climb,Shot,Normal
		EParkourType Type;

	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;

	UPROPERTY(EditAnywhere)  // 몽타쥬의 SectionName
		FName SectionName;

	UPROPERTY(EditAnywhere)
		float MinDistance;

	UPROPERTY(EditAnywhere)
		float MaxDistance;

	UPROPERTY(EditAnywhere)
		float Extent;

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;
public:
	void PlayMontage(class ACharacter* InCharacter);
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U06_BATTLE_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Data")
		UDataTable* DataTable;
private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float  TraceDistance = 600.0f;
	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type>  DebugType;
	UPROPERTY(EditAnywhere, Category = "Trace")
		float AvailableFrontAngle = 15.0f;

public:
	FORCEINLINE bool IsExecuting() { return Type != EParkourType::Max; }

public:
	void  DoParkour(bool bLanded = false);
	void  End_DoParkour();
private:
	bool  Check_Obstacle();   // 1,2,3조건이 만족되는 Check하는 함수

private:  // Climb 
	bool  Check_ClimbMode();
	void  DoParkour_ClimbMode();
	void  End_DoParkour_ClimbMode();

private:  // Fall 
	bool  Check_FallMode();
	void  DoParkour_FallMode();

private:  // Slide 
	bool  Check_SlideMode();
	void  DoParkour_SlideMode();
	void  End_DoParkour_SlideMode();

private:  // Normal,Short,Wall
	bool  Check_ObstacleMode(EParkourType InType, FParkourData& OutData);
	void  DoParkour_ObstacleMode(FParkourData& InData);
	void  End_DoParkour_ObstacleMode();

private:  // 매프레임마다 LineTrace 
	void  CheckTrace_Center();
	void  CheckTrace_Ceil();
	void  CheckTrace_Floor();
	void  CheckTrace_LeftRight();
	void  CheckTrace_Land();
	void  LineTrace(EParkourArrowType InType);

private:
	FHitResult HitResults[(int32)EParkourArrowType::Max];   // Hit결과는 배열로 만듬
private:   // 매 프레임마다 초기화 하는 데이터
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float   HitDistance;
	float   ToFrontYaw;

private:
	EParkourType Type = EParkourType::Max;
	bool	bFalling;
private:
	AActor* BackUpObstacle;

private:  // 외부에서 선언된 변수 저장
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;       // 기존에 구분행이 필요
};
