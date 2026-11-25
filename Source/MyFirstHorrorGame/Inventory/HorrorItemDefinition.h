#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HorrorItemDefinition.generated.h"

class UTexture2D;
class USoundBase;

UCLASS(BlueprintType)
class MYFIRSTHORRORGAME_API UHorrorItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(ClampMin="0.0"))
	float HealAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<USoundBase> UseSound;

	/** Square image shown in the inventory. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon;

	/** Optional inspection artwork. The menu falls back to Icon when unset. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> LargePreviewImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(MultiLine="true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item", meta=(ClampMin="1"))
	int32 MaxStackSize = 1;
};
