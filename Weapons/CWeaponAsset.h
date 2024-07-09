// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponstructures.h"
#include "CWeaponAsset.generated.h"


UCLASS()
class U06_BATTLE_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:  // Attach,DoAction,Equip class define
    UPROPERTY(EditAnywhere)
        TSubclassOf<class ACAttachment> AttachmentClass;
    // ��հ��ϰ��� �ϳ��� �߰��Ѵ�
    UPROPERTY(EditAnywhere)
        FEquipmentData  EquipmentData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment>  EquipmentClass;

    UPROPERTY(EditAnywhere)
        TSubclassOf<class UCDoAction>   DoActionClass;

    UPROPERTY(EditAnywhere)
        TSubclassOf<class UCSubAction>  SubActionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCZAction>	ZActionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCXAction>	XActionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCCAction>	CActionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDownAttack>	DownAttackClass;

    UPROPERTY(EditAnywhere)
        TArray<FDoActionData>          DoActionDatas;
    UPROPERTY(EditAnywhere)
        TArray<FHitData>               HitDatas;   // Collision�� �Ͼ� ���� ä��

public: // Attach,DoAction,Equip class ����
    UCWeaponAsset();
    void  BeginPlay(class ACharacter* InOwner,class UCWeaponData** OutWeaponData);
 //   void  BeginPlay(class ACharacter* InOwner, int Index, class UCWeaponData* OutWeaponData);
 //   OutWeaponData[Index]

public:
    class UCEquipment*  GetEquipment();

};
