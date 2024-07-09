// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

// This class does not need to be modified.
// �Լ�/������ �߰����� �ʴ´�
UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class U06_BATTLE_API IICharacter
{
	GENERATED_BODY()

public:  // �����Լ��� ����� interface�� �����ϴ� ��⿡�� override�ؼ� ���(BP IDamage)
    virtual void End_Avoid() {};  // ���������Լ� X
    virtual void End_Hit()   {};
    virtual void End_Dead()  {};
public:
    void Create_DynamicMaterial(class ACharacter* InCharacter);
    void Change_Color(class ACharacter* InCharacter, FLinearColor InColor);
public:
	virtual void StealthKill() {};
	virtual void StealthDead() {};
};
