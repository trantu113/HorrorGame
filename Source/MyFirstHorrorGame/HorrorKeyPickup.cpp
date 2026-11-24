#include "HorrorKeyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "HorrorKeyRingComponent.h"

AHorrorKeyPickup::AHorrorKeyPickup()
{
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	SetRootComponent(KeyMesh);
	KeyMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	KeyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AHorrorKeyPickup::Interact_Implementation(AActor* Interactor)
{
	if (!IsValid(Interactor))
	{
		return;
	}

	UHorrorKeyRingComponent* Ring = Interactor->FindComponentByClass<UHorrorKeyRingComponent>();
	if (IsValid(Ring) && Ring->TryStoreKey(KeyId))
	{
		Destroy();
	}
}
