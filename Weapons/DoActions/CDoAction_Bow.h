// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Bow.generated.h"


UCLASS(Blueprintable)
class U06_BATTLE_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
	
public:
    UCDoAction_Bow();

private:
    UPROPERTY(EditDefaultsOnly, Category = "Arrow")
        TSubclassOf<class ACArrow> ArrowClass;

private:
    UFUNCTION()
        void OnArrowHit(class AActor* InCasuser, class ACharacter* InOtherCharacter);
    UFUNCTION()
     void OnArrowEndPlay(class ACArrow* InDestroyer);

public: // Actor,AtctorCompont Like�ϰ�
    virtual void BeginPlay
    (
        class ACharacter*   InOwner,
        class ACAttachment* InAttachment,
        class UCEquipment*  InEquipment,
        const TArray<FDoActionData>& InDoActionData,
        const TArray<FHitData>& InHitData
    ) override;
    virtual void Tick(float  InDeltaTime) override;

public:
    virtual void  DoAction() override;
    virtual void  Begin_DoAction() override;
    virtual void  End_DoAction() override;

public:
    virtual void OnBeginEquip() override;
    virtual void OnUnEquip() override;

public:   // Noti
    void   End_BowString();

private:   // Equip��
    void   CreateArrow();
    class  ACArrow* GetAttacedArrow(); // Ȱ�� �߻����� Ȱ�� Create, �׷��� Check�ʿ�

private: // Mesh : Attacment_Bow
    class UPoseableMeshComponent* PoseableMesh; // BeginPlay overide�ذ� ������
    class USkeletalMeshComponent* SkeletalMesh;


private:  // ��Ƽ���� Bowstring
    bool    bAttachedString = true;
    FVector OriginLocation;

private:
    float* Bend = nullptr;
    const  bool* bEquipped;
    TArray<class ACArrow*> Arrows;

};
