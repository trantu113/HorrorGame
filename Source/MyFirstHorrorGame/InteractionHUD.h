#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionHUD.generated.h"

class UImage;
class UTextBlock;

UCLASS(Abstract, Blueprintable)
class UInteractionHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInteractionAvailable(bool bAvailable);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> AimDot;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> InteractPrompt;

	TOptional<bool> LastAvailability;
};
