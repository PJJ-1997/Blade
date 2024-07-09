// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CZAction.h"
#include "Global.h"
#include "CZAction.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

UCZAction::UCZAction()
{

}
void UCZAction::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCDoAction * InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCZAction::Pressed()
{
	bInAction = true;
}

void UCZAction::Released()
{
	bInAction = false;
}