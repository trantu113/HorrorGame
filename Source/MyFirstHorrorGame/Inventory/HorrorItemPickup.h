#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorInteractable.h"
#include "HorrorItemPickup.generated.h"

class UHorrorItemDefinition;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class MYFIRSTHORRORGAME_API AHorrorItemPickup : public AActor, public IHorrorInteractable
{
	GENERATED_BODY()

public:
	AHorrorItemPickup();
	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	TObjectPtr<UStaticMeshComponent> PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	TObjectPtr<UHorrorItemDefinition> Item;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup", meta=(ClampMin="1"))
	int32 Quantity = 1;

	UFUNCTION(BlueprintImplementableEvent, Category="Pickup")
	void OnInsufficientSpace(AActor* Interactor);

private:
	bool bCollecting = false;
};
