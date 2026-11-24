#include "HorrorLockedDoor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HorrorKeyRingComponent.h"

AHorrorLockedDoor::AHorrorLockedDoor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("DoorFrame")));
	Hinge = CreateDefaultSubobject<USceneComponent>(TEXT("Hinge"));
	Hinge->SetupAttachment(RootComponent);
	DoorLeaf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeaf"));
	DoorLeaf->SetupAttachment(Hinge);
	DoorLeaf->SetMobility(EComponentMobility::Movable);
	DoorLeaf->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void AHorrorLockedDoor::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(Interactor))
	{
		return;
	}

	if (bIsOpen)
	{
		CloseDoor();
		return;
	}

	const UHorrorKeyRingComponent* Ring = Interactor->FindComponentByClass<UHorrorKeyRingComponent>();
	if (!IsValid(Ring) || !Ring->HasKey(RequiredKeyId))
	{
		OnDoorLocked();
		return;
	}

	bIsOpen = true;
	ClosedHingeRotation = Hinge->GetRelativeRotation();
	Hinge->AddLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
	OnDoorOpened();
}

void AHorrorLockedDoor::CloseDoor()
{
	bIsOpen = false;
	Hinge->SetRelativeRotation(ClosedHingeRotation);
	OnDoorClosed();
}
