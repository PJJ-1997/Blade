// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/CStateComponent.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData : public FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere)
        EStateType Type;
    UPROPERTY(EditAnywhere)
        class UAnimMontage* Montage;
    UPROPERTY(EditAnywhere)
        float PlayRate = 1.0f;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U06_BATTLE_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "DataTable")
        UDataTable* DataTable;
//	UFUNCTION()
//		void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
//	UFUNCTION()
//		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
    void PlayBackStepMode();
    void PlayDeadMode();
    void PlayRollMode();
private:
    void PlayAnimMontage(EStateType InType);

private: 
    FMontagesData* Datas[(int32)EStateType::Max];  // 데이터는 포인터형으로 return 받음
private:
    class ACharacter* OwnerCharacter;  // 각 콤포넌트마다 생성시 변수를 저장
public:
	void PlayFKeyMontage();
	void PlayBlockAnimation();
private:
	// 재생할 애니메이션 몽타주를 위한 UAnimMontage 포인터
	UAnimMontage* FKeyMontage;
	UAnimMontage* BlockMontage;
};
