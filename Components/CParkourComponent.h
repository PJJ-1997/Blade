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

// ���������� ���̺�
USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

		// �������� BP���� ������ ���� --> TMAP���� ����
public:
	UPROPERTY(EditAnywhere) // Climb,Shot,Normal
		EParkourType Type;

	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1;

	UPROPERTY(EditAnywhere)  // ��Ÿ���� SectionName
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
	bool  Check_Obstacle();   // 1,2,3������ �����Ǵ� Check�ϴ� �Լ�

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

private:  // �������Ӹ��� LineTrace 
	void  CheckTrace_Center();
	void  CheckTrace_Ceil();
	void  CheckTrace_Floor();
	void  CheckTrace_LeftRight();
	void  CheckTrace_Land();
	void  LineTrace(EParkourArrowType InType);

private:
	FHitResult HitResults[(int32)EParkourArrowType::Max];   // Hit����� �迭�� ����
private:   // �� �����Ӹ��� �ʱ�ȭ �ϴ� ������
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float   HitDistance;
	float   ToFrontYaw;

private:
	EParkourType Type = EParkourType::Max;
	bool	bFalling;
private:
	AActor* BackUpObstacle;

private:  // �ܺο��� ����� ���� ����
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;       // ������ �������� �ʿ�
};
