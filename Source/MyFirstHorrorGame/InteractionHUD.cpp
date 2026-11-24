#include "InteractionHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UInteractionHUD::SetInteractionAvailable(bool bAvailable)
{
	if (!IsValid(AimDot) || !IsValid(InteractPrompt))
	{
		return;
	}

	if (LastAvailability.IsSet() && LastAvailability.GetValue() == bAvailable)
	{
		return;
	}

	LastAvailability = bAvailable;
	AimDot->SetRenderOpacity(bAvailable ? 1.0f : 0.4f);
	InteractPrompt->SetVisibility(bAvailable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
