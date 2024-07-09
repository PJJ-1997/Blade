// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CXAction.h"
#include "Global.h"
#include "CXAction.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

UCXAction::UCXAction()
{

}
void UCXAction::BeginPlay(ACharacter * InOwner, ACAttachment * InAttachment, UCDoAction * InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
}

void UCXAction::Pressed()
{
	bInAction = true;
}

void UCXAction::Released()
{
	bInAction = false;
}
