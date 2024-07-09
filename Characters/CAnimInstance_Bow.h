// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

public:
    void NativeBeginPlay() override;
    void NativeUpdateAnimation(float DeltaSeconds) override;

protected: // EditAnywhere : AnimGraph에서 사용가능하게끔 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Bend;


public: // Bend값을 포인터로 return : ==> 변경시에도 다른함수레서 그 값을 바고 바로 사용 가능
    FORCEINLINE float* GetBend() { return &Bend; }


};