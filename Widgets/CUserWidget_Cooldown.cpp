// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CUserWidget_Cooldown.h"

void UCUserWidget_Cooldown::UpdateCooldown(FName SkillName, float CooldownTime)
{
	OnCooldownUpdate(SkillName, CooldownTime);
}

