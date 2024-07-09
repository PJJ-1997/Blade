// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_NeedItem.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCUserWidget_NeedItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UButton* CheckButtonUI;

private:
	UFUNCTION(BlueprintCallable)
		void CheckButtonCallback();
};
