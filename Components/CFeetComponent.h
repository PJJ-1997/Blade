
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "CFeetComponent.generated.h"


USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector LeftDistance;   // X축 : 높이
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector RightDistance;  // X축 : 높이
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FVector PelvisDistance;  // Z축 : 높이
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FRotator LeftRotation;
	UPROPERTY(BlueprintReadOnly, Category = "Feet")
		FRotator RightRotation;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U06_BATTLE_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCFeetComponent();
protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float InterpSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float OffsetDistance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName LeftSocketName = "foot_l";

	UPROPERTY(EditAnywhere, Category = "Trace")
		FName RightSocketName = "foot_r";

public:
	FORCEINLINE FFeetData GetData() { return Data; }
private:
	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);

private:
	FFeetData Data;
	class ACharacter* OwnerCharacter;
};
