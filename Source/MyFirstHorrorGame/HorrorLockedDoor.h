#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorrorInteractable.h"
#include "HorrorLockedDoor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class MYFIRSTHORRORGAME_API AHorrorLockedDoor : public AActor, public IHorrorInteractable
{
	GENERATED_BODY()

public:
	AHorrorLockedDoor();

	virtual void Interact_Implementation(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> DoorLeaf;

	/** ID of the key that unlocks this door. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Door")
	FName RequiredKeyId = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> Hinge;

	/** Blueprint can play opening feedback. */
	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorOpened();

	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorClosed();

	/** Blueprint can tell the player which key they need. */
	UFUNCTION(BlueprintImplementableEvent, Category="Door")
	void OnDoorLocked();

private:
	void CloseDoor();
	FRotator ClosedHingeRotation = FRotator::ZeroRotator;
	bool bIsOpen = false;
};
