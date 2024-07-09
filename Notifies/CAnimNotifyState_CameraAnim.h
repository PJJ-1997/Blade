// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAnim.generated.h"


UCLASS()
class U06_BATTLE_API UCAnimNotifyState_CameraAnim : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
    FString GetNotifyName_Implementation() const override;
    virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

private:
    UPROPERTY(EditAnywhere, Category = "Camera")
        class UCameraAnim* CameraAnim;   // BP»ó¿¡¼­ CA_Fist
    UPROPERTY(EditAnywhere, Category = "Camera")
        float PlayRatio = 5.0f;         
    UPROPERTY(EditAnywhere, Category = "Camera")
        float BlendInTime = 0.0f;
    UPROPERTY(EditAnywhere, Category = "Camera")
        float BlendOutTime = 0.0f;


private:
    UPROPERTY(EditAnywhere, Category = "Camera")
        TSoftObjectPtr<class ACharacter> Target;


private:
    UPROPERTY(EditAnywhere, Category = "Modifier")
        FVector LocationOffset = FVector(-400,0,90);

    UPROPERTY(EditAnywhere, Category = "Modifier")
        float PitchOffset = -15.0f;

private:
    class UCCameraModifier* Modifier;
};

