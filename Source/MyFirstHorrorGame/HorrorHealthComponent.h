#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "HorrorHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHorrorHealthChanged, float, CurrentHealth, float, MaxHealth, float, HealthRatio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHorrorPlayerDied);

UCLASS(ClassGroup=(Gameplay), meta=(BlueprintSpawnableComponent))
class MYFIRSTHORRORGAME_API UHorrorHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Health")
	FHorrorHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Health")
	FHorrorPlayerDied OnPlayerDied;

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeTestDamage(float Amount = 20.0f);

	UFUNCTION(BlueprintPure, Category="Health")
	bool CanHeal(float Amount) const;

	UFUNCTION(BlueprintCallable, Category="Health")
	bool Heal(float Amount);

	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealthRatio() const { return CurrentHealth / MaxHealth; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health", meta=(ClampMin="1", AllowPrivateAccess="true"))
	float MaxHealth = 100.0f;

	UPROPERTY(Transient, BlueprintReadOnly, Category="Health", meta=(AllowPrivateAccess="true"))
	float CurrentHealth = 100.0f;

	void ApplyHealth(float Value);

	UFUNCTION()
	void HandleDamage(AActor* Actor, float Damage, const UDamageType* Type, AController* InstigatorController, AActor* Causer);
};
