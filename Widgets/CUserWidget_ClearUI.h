// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_ClearUI.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCUserWidget_ClearUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* RetryButtonUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* ExitButtonUI;

private:
	// ��ư�� ������ ��, ȣ��� ��������Ʈ�� ����� �Լ�
	UFUNCTION(BlueprintCallable)
		void RetryButtonCallback();

	UFUNCTION(BlueprintCallable)
		void ExitButtonCallback();
};
