#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HorrorKeyRingComponent.generated.h"

/** Stores the key IDs collected by the actor that owns this component. */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class MYFIRSTHORRORGAME_API UHorrorKeyRingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Accept a valid key ID, even if it is already on the ring. */
	bool TryStoreKey(FName KeyId);

	/** Check for a matching key without consuming it. */
	bool HasKey(FName KeyId) const;

private:
	UPROPERTY(Transient)
	TSet<FName> CollectedKeyIds;
};
