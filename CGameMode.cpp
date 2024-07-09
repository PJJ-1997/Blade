
#include "CGameMode.h"
#include "Global.h"
#include "UserInterface/CSTutorialHUD.h"

ACGameMode::ACGameMode()
{
    CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
    
}
void ACGameMode::BeginPlay()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
	}
}
void ACGameMode::OnBossDead()
{
	ACSTutorialHUD* HUD = Cast<ACSTutorialHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (HUD)
	{
		HUD->ShowClearGameWidget();
	}
}