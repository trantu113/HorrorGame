#include "HorrorItemPickup.h"
#include "Components/StaticMeshComponent.h"
#include "HorrorInventoryComponent.h"

AHorrorItemPickup::AHorrorItemPickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	SetRootComponent(PickupMesh);
	PickupMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	PickupMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AHorrorItemPickup::Interact_Implementation(AActor* Interactor)
{
	if (bCollecting || !IsValid(Interactor) || !IsValid(Item) || Quantity <= 0 || Item->MaxStackSize <= 0)
	{
		return;
	}

	UHorrorInventoryComponent* Inventory = Interactor->FindComponentByClass<UHorrorInventoryComponent>();
	if (!IsValid(Inventory))
	{
		return;
	}

	bCollecting = true;
	if (Inventory->TryAddItem(Item, Quantity))
	{
		Destroy();
	}
	else
	{
		OnInsufficientSpace(Interactor);
		bCollecting = false;
	}
}
