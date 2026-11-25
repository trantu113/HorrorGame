#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HorrorInventoryMenu.generated.h"

class UHorrorInventoryComponent;
class UHorrorInventorySlotWidget;
class UHorrorItemDefinition;
class UImage;
class UTextBlock;
class UUniformGridPanel;

UCLASS(Abstract, Blueprintable)
class MYFIRSTHORRORGAME_API UHorrorInventoryMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	bool SetInventory(UHorrorInventoryComponent* InInventory);
	void ShowItem(UHorrorInventorySlotWidget* Slot, const UHorrorItemDefinition* Item);
	bool UseSlot(UHorrorInventorySlotWidget* Slot);
	void ClearItem(const UHorrorInventorySlotWidget* Slot);

protected:
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void RefreshGrid();

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> SlotGrid;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UImage> ItemPreview;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UHorrorInventorySlotWidget> SlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory", meta=(ClampMin="1"))
	int32 Columns = 3;

	TWeakObjectPtr<UHorrorInventoryComponent> Inventory;
	TWeakObjectPtr<UHorrorInventorySlotWidget> HoveredSlot;
};
