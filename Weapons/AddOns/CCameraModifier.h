// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CCameraModifier.generated.h"

/**
 * 
 */
UCLASS()
class U06_BATTLE_API UCCameraModifier : public UCameraModifier
{
	GENERATED_BODY()
	
protected:
    /** Allows modifying the camera in native code. */
    virtual void ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV)  override;

public:
    FORCEINLINE void SetLocation(const FVector& InLocation) { Location = InLocation; }
    FORCEINLINE void SetRotation(const FRotator& InRotation) { Rotation = InRotation; }

private:
    FVector  Location;
    FRotator Rotation;
};
