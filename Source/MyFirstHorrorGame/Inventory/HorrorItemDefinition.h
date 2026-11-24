#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HorrorItemDefinition.generated.h"

class UTexture2D;

UCLASS(BlueprintType)
class MYFIRSTHORRORGAME_API UHorrorItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText DisplayName;

	/** Square image shown in the inventory. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(MultiLine="true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(ClampMin="1"))
	int32 MaxStackSize = 1;
};
