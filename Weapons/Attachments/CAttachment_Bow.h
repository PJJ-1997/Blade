// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"


UCLASS()
class U06_BATTLE_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

public:
    ACAttachment_Bow();
protected:
    virtual void BeginPlay() override;
public:
    virtual void Tick(float DelltaTime) override;
	
private: // Component
    UPROPERTY(VisibleAnywhere)
        class USkeletalMeshComponent* SkeletalMesh;

    UPROPERTY(VisibleAnywhere)
        class UPoseableMeshComponent* PoseableMesh;

private: // View Min,Max
    UPROPERTY(EditDefaultsOnly, Category = "View")
        FVector2D ViewPitchRange = FVector2D(-40.0f, 30.0f);

public:
    virtual    void OnBeginEquip_Implementation() override;   // C에서는 정의 
    virtual    void OnUnEquip_Implementation() override;
public:
    float* GetBend();
private:
    FVector2D OriginViewPitchRange;

};
