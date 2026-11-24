#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorInteractable.h"
#include "HorrorKeyPickup.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class MYFIRSTHORRORGAME_API AHorrorKeyPickup : public AActor, public IHorrorInteractable
{
	GENERATED_BODY()

public:
	AHorrorKeyPickup();

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> KeyMesh;

	/** Match this ID to the door's Required Key Id. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Key")
	FName KeyId = NAME_None;
};
