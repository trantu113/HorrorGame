#include "HorrorInventorySlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "HorrorInventoryMenu.h"
#include "HorrorItemDefinition.h"

void UHorrorInventorySlotWidget::SetSlot(UHorrorInventoryMenu* InMenu, UHorrorItemDefinition* InItem, int32 Quantity)
{
	Menu = InMenu;
	Item = InItem;

	UTexture2D* Icon = IsValid(InItem) ? InItem->Icon.Get() : nullptr;
	ItemIcon->SetBrushFromTexture(Icon);
	ItemIcon->SetVisibility(IsValid(Icon) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	const bool bShowLabel = IsValid(InItem) && !IsValid(Icon);
	ItemLabel->SetText(bShowLabel ? InItem->DisplayName : FText::GetEmpty());
	ItemLabel->SetVisibility(bShowLabel ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	StackCount->SetText(FText::Format(NSLOCTEXT("HorrorInventory", "StackCount", "x{0}"), FText::AsNumber(Quantity)));
	StackCount->SetVisibility(IsValid(InItem) && Quantity > 1
		? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UHorrorInventorySlotWidget::NativeOnMouseEnter(const FGeometry& Geometry, const FPointerEvent& Event)
{
	Super::NativeOnMouseEnter(Geometry, Event);
	if (UHorrorInventoryMenu* InventoryMenu = Menu.Get())
	{
		InventoryMenu->ShowItem(this, Item.Get());
	}
}

void UHorrorInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& Event)
{
	Super::NativeOnMouseLeave(Event);
	if (UHorrorInventoryMenu* InventoryMenu = Menu.Get())
	{
		InventoryMenu->ClearItem(this);
	}
}
