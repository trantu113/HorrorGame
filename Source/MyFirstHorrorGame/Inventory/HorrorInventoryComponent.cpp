#include "HorrorInventoryComponent.h"

bool UHorrorInventoryComponent::TryAddItem(UHorrorItemDefinition* Item, int32 Quantity)
{
	if (!IsValid(Item) || Quantity <= 0 || Item->MaxStackSize <= 0)
	{
		return false;
	}

	const int32 StackLimit = Item->MaxStackSize;
	const int32 FreeSlots = FMath::Max(0, GetSlotCapacity() - Slots.Num());
	int64 Space = static_cast<int64>(FreeSlots) * StackLimit;
	for (const FHorrorInventorySlot& Slot : Slots)
	{
		if (Slot.Item == Item)
		{
			Space += FMath::Max(0, StackLimit - Slot.Quantity);
		}
	}
	if (Space < Quantity)
	{
		return false;
	}

	int32 Remaining = Quantity;
	for (FHorrorInventorySlot& Slot : Slots)
	{
		if (Slot.Item == Item && Slot.Quantity < StackLimit)
		{
			const int32 Added = FMath::Min(Remaining, StackLimit - Slot.Quantity);
			Slot.Quantity += Added;
			Remaining -= Added;
		}
	}
	while (Remaining > 0)
	{
		FHorrorInventorySlot& Slot = Slots.AddDefaulted_GetRef();
		Slot.Item = Item;
		Slot.Quantity = FMath::Min(Remaining, StackLimit);
		Remaining -= Slot.Quantity;
	}
	OnInventoryChanged.Broadcast();
	return true;
}

int64 UHorrorInventoryComponent::CountItem(UHorrorItemDefinition* Item) const
{
	int64 Count = 0;
	if (IsValid(Item))
	{
		for (const FHorrorInventorySlot& Slot : Slots)
		{
			if (Slot.Item == Item)
			{
				Count += Slot.Quantity;
			}
		}
	}
	return Count;
}

bool UHorrorInventoryComponent::TryRemoveItem(UHorrorItemDefinition* Item, int32 Quantity)
{
	if (Quantity <= 0 || CountItem(Item) < Quantity)
	{
		return false;
	}

	for (int32 Index = Slots.Num() - 1; Index >= 0 && Quantity > 0; --Index)
	{
		FHorrorInventorySlot& Slot = Slots[Index];
		if (Slot.Item == Item)
		{
			const int32 Removed = FMath::Min(Quantity, Slot.Quantity);
			Slot.Quantity -= Removed;
			Quantity -= Removed;
			if (Slot.Quantity == 0)
			{
				Slots.RemoveAt(Index);
			}
		}
	}
	OnInventoryChanged.Broadcast();
	return true;
}
