#include "HorrorInventoryMenu.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameFramework/PlayerController.h"
#include "HorrorInventoryComponent.h"
#include "HorrorInventorySlotWidget.h"

bool UHorrorInventoryMenu::SetInventory(UHorrorInventoryComponent* InInventory)
{
	if (!IsValid(InInventory) || !SlotWidgetClass)
	{
		return false;
	}

	if (UHorrorInventoryComponent* Previous = Inventory.Get())
	{
		Previous->OnInventoryChanged.RemoveDynamic(this, &UHorrorInventoryMenu::RefreshGrid);
	}
	Inventory = InInventory;
	InInventory->OnInventoryChanged.AddUniqueDynamic(this, &UHorrorInventoryMenu::RefreshGrid);
	RefreshGrid();
	return true;
}

void UHorrorInventoryMenu::NativeDestruct()
{
	if (UHorrorInventoryComponent* Source = Inventory.Get())
	{
		Source->OnInventoryChanged.RemoveDynamic(this, &UHorrorInventoryMenu::RefreshGrid);
	}
	Inventory.Reset();
	Super::NativeDestruct();
}

void UHorrorInventoryMenu::RefreshGrid()
{
	UHorrorInventoryComponent* Source = Inventory.Get();
	APlayerController* Player = GetOwningPlayer();
	if (!IsValid(Source) || !IsValid(Player))
	{
		return;
	}
	ShowItem(nullptr, nullptr);
	SlotGrid->ClearChildren();

	const TArray<FHorrorInventorySlot>& Slots = Source->GetSlots();
	const int32 ColumnCount = FMath::Max(1, Columns);
	for (int32 Index = 0; Index < Source->GetSlotCapacity(); ++Index)
	{
		UHorrorInventorySlotWidget* Widget = CreateWidget<UHorrorInventorySlotWidget>(Player, SlotWidgetClass);
		if (IsValid(Widget))
		{
			const FHorrorInventorySlot* Slot = Slots.IsValidIndex(Index) ? &Slots[Index] : nullptr;
			Widget->SetSlot(this, Slot ? Slot->Item.Get() : nullptr, Slot ? Slot->Quantity : 0);
			SlotGrid->AddChildToUniformGrid(Widget, Index / ColumnCount, Index % ColumnCount);
		}
	}
}

void UHorrorInventoryMenu::ShowItem(UHorrorInventorySlotWidget* Slot, const UHorrorItemDefinition* Item)
{
	HoveredSlot = Slot;
	ItemNameText->SetText(IsValid(Item) ? Item->DisplayName : FText::GetEmpty());
	ItemDescriptionText->SetText(IsValid(Item) ? Item->Description : FText::GetEmpty());
}

void UHorrorInventoryMenu::ClearItem(const UHorrorInventorySlotWidget* Slot)
{
	if (HoveredSlot.Get() == Slot)
	{
		ShowItem(nullptr, nullptr);
	}
}
