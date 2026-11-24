#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HorrorInteractable.generated.h"

class AActor;

UINTERFACE(BlueprintType, Blueprintable)
class UHorrorInteractable : public UInterface
{
	GENERATED_BODY()
};

class IHorrorInteractable
{
	GENERATED_BODY()

public:
	/** Respond to an interaction from the supplied actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void Interact(AActor* Interactor);
};
