#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HorrorGameOverWidget.generated.h"

class UButton;

UCLASS(Abstract, Blueprintable)
class MYFIRSTHORRORGAME_API UHorrorGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> PlayAgainButton;

	UFUNCTION()
	void RestartLevel();
};
