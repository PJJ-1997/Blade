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

protected: // EditAnywhere : AnimGraph���� ��밡���ϰԲ� 
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
        float Bend;


public: // Bend���� �����ͷ� return : ==> ����ÿ��� �ٸ��Լ����� �� ���� �ٰ� �ٷ� ��� ����
    FORCEINLINE float* GetBend() { return &Bend; }


};