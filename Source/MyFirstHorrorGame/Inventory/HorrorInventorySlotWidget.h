#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HorrorInventorySlotWidget.generated.h"

class UHorrorInventoryMenu;
class UHorrorItemDefinition;
class UBorder;
class UImage;
class UTextBlock;

UCLASS(Abstract, Blueprintable)
class MYFIRSTHORRORGAME_API UHorrorInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlot(UHorrorInventoryMenu* InMenu, UHorrorItemDefinition* InItem, int32 Quantity);

protected:
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual void NativeOnMouseEnter(const FGeometry& Geometry, const FPointerEvent& Event) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& Event) override;

private:
	void SetHighlightVisible(bool bVisible);

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UBorder> HoverOutline;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> StackCount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ItemLabel;

	TWeakObjectPtr<UHorrorInventoryMenu> Menu;
	TWeakObjectPtr<UHorrorItemDefinition> Item;
};
