// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/AddOns/CCameraModifier.h"
#include "Global.h"

    /** Allows modifying the camera in native code. */
void UCCameraModifier::ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV)
{

    Super::ModifyCamera(DeltaTime, ViewLocation, ViewRotation, FOV, NewViewLocation, NewViewRotation, NewFOV);

    CLog::Print(ViewLocation, 0);
    CLog::Print(ViewRotation, 0);

 //   ViewRotation = ViewRotation;
 //   ViewRotation = ViewRotation;

    ViewLocation = Location;
    ViewRotation = Rotation;
}

