// Fill out your copyright notice in the Description page of Project Settings.


#include "World/CAudioTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"


ACAudioTrigger::ACAudioTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
	AudioComponent->bAutoActivate = true; // 자동 재생 설정
	//TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//RootComponent = TriggerBox;

	//AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	//AudioComponent->SetupAttachment(RootComponent);
	//AudioComponent->bAutoActivate = true;

	//TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACAudioTrigger::OnOverlapBegin);
	//TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACAudioTrigger::OnOverlapEnd);
}


void ACAudioTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (SoundCue && AudioComponent)
	{
		AudioComponent->SetSound(SoundCue);
		AudioComponent->Play();
	}
	
}

void ACAudioTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ACAudioTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor && (OtherActor != this) && OtherComp)
//	{
//		if (SoundCue && AudioComponent)
//		{
//			AudioComponent->SetSound(SoundCue);
//			AudioComponent->Play();
//		}
//	}
//}
//
//void ACAudioTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (OtherActor && (OtherActor != this) && OtherComp)
//	{
//		if (AudioComponent)
//		{
//			AudioComponent->Stop();
//		}
//	}
//}