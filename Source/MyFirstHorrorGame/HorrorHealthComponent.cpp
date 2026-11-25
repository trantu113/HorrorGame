#include "HorrorHealthComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"

void UHorrorHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	MaxHealth = FMath::Max(1.0f, MaxHealth);
	CurrentHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHorrorHealthComponent::HandleDamage);
}

void UHorrorHealthComponent::EndPlay(const EEndPlayReason::Type Reason)
{
	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UHorrorHealthComponent::HandleDamage);
	Super::EndPlay(Reason);
}

void UHorrorHealthComponent::HandleDamage(AActor*, float Damage, const UDamageType*, AController*, AActor*)
{
	if (Damage > 0 && FMath::IsFinite(Damage))
	{
		ApplyHealth(CurrentHealth - Damage);
	}
}

void UHorrorHealthComponent::TakeTestDamage(float Amount)
{
	if (IsValid(GetOwner()) && Amount > 0 && FMath::IsFinite(Amount))
	{
		FDamageEvent Event;
		GetOwner()->TakeDamage(Amount, Event, nullptr, nullptr);
	}
}

bool UHorrorHealthComponent::CanHeal(float Amount) const
{
	return FMath::IsFinite(Amount) && Amount > 0 && CurrentHealth > 0 && CurrentHealth < MaxHealth;
}

bool UHorrorHealthComponent::Heal(float Amount)
{
	if (!CanHeal(Amount))
	{
		return false;
	}
	ApplyHealth(CurrentHealth + Amount);
	return true;
}

void UHorrorHealthComponent::ApplyHealth(float Value)
{
	const float Next = FMath::Clamp(Value, 0.0f, MaxHealth);
	if (Next == CurrentHealth)
	{
		return;
	}
	CurrentHealth = Next;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth, GetHealthRatio());
	if (Next == 0.0f)
	{
		OnPlayerDied.Broadcast();
	}
}
