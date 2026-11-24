#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HorrorItemDefinition.h"
#include "HorrorInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FHorrorInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	TObjectPtr<UHorrorItemDefinition> Item;

	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	int32 Quantity = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorrorInventoryChanged);

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class MYFIRSTHORRORGAME_API UHorrorInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool TryAddItem(UHorrorItemDefinition* Item, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool TryRemoveItem(UHorrorItemDefinition* Item, int32 Quantity);

	UFUNCTION(BlueprintPure, Category="Inventory")
	int64 CountItem(UHorrorItemDefinition* Item) const;

	int32 GetSlotCapacity() const { return FMath::Clamp(SlotCapacity, 1, 64); }
	const TArray<FHorrorInventorySlot>& GetSlots() const { return Slots; }

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnHorrorInventoryChanged OnInventoryChanged;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(ClampMin="1", ClampMax="64", AllowPrivateAccess="true"))
	int32 SlotCapacity = 6;

	/** Occupied slots only. Emptying a stack frees its slot. */
	UPROPERTY(Transient, BlueprintReadOnly, Category="Inventory", meta=(AllowPrivateAccess="true"))
	TArray<FHorrorInventorySlot> Slots;
};
