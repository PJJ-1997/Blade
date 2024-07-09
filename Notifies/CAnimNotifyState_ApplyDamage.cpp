// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_ApplyDamage.h"
#include "Characters/CPlayer.h"
#include "Characters/CBoss.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"

void UCAnimNotifyState_ApplyDamage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (ACBoss* Boss = Cast<ACBoss>(Character))
		{
			// 데미지 관련 변수를 설정합니다.
			DamageAmount = 500.0f; // 원하는 데미지 값으로 설정
			DamageEvent = FDamageEvent();
			EventInstigator = Boss->GetController();
			DamageCauser = Boss;

			UE_LOG(LogTemp, Log, TEXT("NotifyBegin: DamageAmount = %f, EventInstigator = %s, DamageCauser = %s"),
				DamageAmount,
				EventInstigator ? *EventInstigator->GetName() : TEXT("nullptr"),
				DamageCauser ? *DamageCauser->GetName() : TEXT("nullptr"));
		}
	}
}

void UCAnimNotifyState_ApplyDamage::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (ACBoss* Boss = Cast<ACBoss>(Character))
		{
			// 공격 범위 내 플레이어 탐지
			TArray<AActor*> OverlappingActors;
			Boss->GetOverlappingActors(OverlappingActors, ACPlayer::StaticClass());

			for (AActor* Actor : OverlappingActors)
			{
				if (ACPlayer* Player = Cast<ACPlayer>(Actor))
				{
					// 플레이어에게 데미지를 주는 함수 호출
					float DamageDealt = Player->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

					// 디버깅 출력 추가
					UE_LOG(LogTemp, Log, TEXT("NotifyTick: Player = %s, DamageAmount = %f, DamageDealt = %f"),
						*Player->GetName(),
						DamageAmount,
						DamageDealt);
				}
			}
		}
	}
}

void UCAnimNotifyState_ApplyDamage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Log, TEXT("NotifyEnd Called"));

	
}
