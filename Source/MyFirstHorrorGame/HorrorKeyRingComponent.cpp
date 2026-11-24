#include "HorrorKeyRingComponent.h"

bool UHorrorKeyRingComponent::TryStoreKey(FName KeyId)
{
	if (KeyId.IsNone())
	{
		return false;
	}

	CollectedKeyIds.Add(KeyId);
	return true;
}

bool UHorrorKeyRingComponent::HasKey(FName KeyId) const
{
	if (KeyId.IsNone())
	{
		return false;
	}

	return CollectedKeyIds.Contains(KeyId);
}
