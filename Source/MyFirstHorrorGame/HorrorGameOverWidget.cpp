#include "HorrorGameOverWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UHorrorGameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IsValid(PlayAgainButton))
	{
		PlayAgainButton->OnClicked.AddUniqueDynamic(this, &UHorrorGameOverWidget::RestartLevel);
	}
}

void UHorrorGameOverWidget::RestartLevel()
{
	APlayerController* Player = GetOwningPlayer();
	if (!IsValid(Player))
	{
		return;
	}

	PlayAgainButton->SetIsEnabled(false);
	Player->SetInputMode(FInputModeGameOnly());
	Player->bShowMouseCursor = false;
	const FString Map = UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, FName(*Map));
}
