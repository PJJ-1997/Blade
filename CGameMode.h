// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
    ACGameMode();
private:
	virtual void BeginPlay() override;
public:
	void OnBossDead();
};
