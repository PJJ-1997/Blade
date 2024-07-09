// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"


UCLASS()
class U06_BATTLE_API UCWeaponData : public UObject
{
	GENERATED_BODY()
private:
    friend class UCWeaponAsset;
private: // Object이므로 직렬화
    UPROPERTY()
        class ACAttachment* Attachment;
    UPROPERTY()
        class UCEquipment*  Equipment;
    UPROPERTY()
        class UCDoAction*   DoAction;
    UPROPERTY()
        class UCSubAction*  SubAction;
	UPROPERTY()
		class UCZAction*	ZKeyAction;
	UPROPERTY()
		class UCXAction*	XKeyAction;
	UPROPERTY()
		class UCCAction*	CKeyAction;
	UPROPERTY()
		class UCDownAttack* QKeyAction;

public:
    void  SetAttachment(class ACAttachment* InAttachment);
    void  SetEquipment(class  UCEquipment * InEquipment);
public:
    FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
    FORCEINLINE class UCEquipment*  GetEquipment()  { return Equipment; }
    FORCEINLINE class UCDoAction*   GetDoAction()   { return DoAction; }
    FORCEINLINE class UCSubAction*  GetSubAction()  { return SubAction; }
	FORCEINLINE class UCZAction*    GetZKeyAction() { return ZKeyAction; }
	FORCEINLINE class UCXAction*    GetXKeyAction() { return XKeyAction; }
	FORCEINLINE class UCCAction*	GetCKeyAction() { return CKeyAction; }
	FORCEINLINE class UCDownAttack* GetQKeyAction() { return QKeyAction; }

};
