
#include "Components/CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"


UCStatusComponent::UCStatusComponent()
{
	Health = MaxHealth; // 초기화
	HealSound = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundCue'/Game/Assets154/Assets154/Content/Ultimate_SFX/Ultimate_Water_Sounds/Cues/Drinking_Water_4_Cue.Drinking_Water_4_Cue'"));
	HealSound->bAutoActivate = false; // 초기화 시 자동으로 재생되지 않도록 설정
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

    OwnerCharacter = Cast<ACharacter>(GetOwner());
    Health = MaxHealth;
	
}

void UCStatusComponent::Damage(float InAmount)
{
    Health += (InAmount*-1.0f);
    Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SetHealth(float InHealth)
{
	Health = FMath::Clamp(InHealth, 0.0f, MaxHealth);
}

void UCStatusComponent::Heal(float InAmount)
{
	Health = FMath::Clamp(Health + InAmount, 0.0f, MaxHealth);

	if (HealSound != nullptr && HealSoundCue != nullptr)
	{
		// 소리를 설정하고 재생
		HealSound->SetSound(HealSoundCue);
		HealSound->Play();
	}
}

