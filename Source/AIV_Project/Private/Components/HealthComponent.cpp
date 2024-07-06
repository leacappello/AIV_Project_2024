// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "AIV_Project/Public/Utils.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	StartHp = 100.f;
	bAlive = true;
	bCanTakeDamage = true;

	FlushOldDamagesInterval = 5.f;
	FlushOldDamagesTime = 10.f;
}

void UHealthComponent::LoadComponent_Implementation(const FComponentData& ComponentSaveData)
{
	ISavable::LoadComponent_Implementation(ComponentSaveData);
	OnHpChange.Broadcast(nullptr, 0.f, Hp);
	if (Hp <= 0.f)
	{
		Die(GetOwner());
	}
}

void UHealthComponent::Damage(UObject* Instigator, float Damage)
{
	if (!bCanTakeDamage && !bAlive)
		return;
	
	Hp -= Damage;
	OnHpChange.Broadcast(Instigator, Damage, Hp);
	if (Hp <= 0.f)
	{
		Die(Instigator);
	}
	
	if (LastDamages.Contains(Instigator))
		LastDamages[Instigator] = GetWorld()->GetTimeSeconds();
	else
		LastDamages.Add(Instigator, GetWorld()->GetTimeSeconds());
}

void UHealthComponent::Die(UObject* Instigator)
{
	bAlive = false;
	OnDeath.Broadcast(Instigator);
}

void UHealthComponent::Heal(UObject* Instigator, float HpAmount)
{
	Hp += HpAmount;
	Hp = FMath::Clamp(Hp, 0.f, StartHp);
	OnHpChange.Broadcast(Instigator, HpAmount, Hp);
}

void UHealthComponent::Revive()
{
	bAlive = true;
	Hp = StartHp;
	OnRevive.Broadcast();
}

float UHealthComponent::GetLastDamageTime(UObject* Instigator) const
{
	if (LastDamages.Contains(Instigator))
	{
		return LastDamages[Instigator];
	}
	return -1.f;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Hp = StartHp;
	bAlive = true;
	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LastFlushTime + FlushOldDamagesInterval < GetWorld()->GetTimeSeconds())
	{
		LOGWARN("Flushing old damages")
		
		TArray<UObject*> KeysToRemove;
		for (auto Element : LastDamages)
		{
			if (Element.Value + FlushOldDamagesTime < GetWorld()->GetTimeSeconds())
			{
				KeysToRemove.Add(Element.Key);
			}
		}
		
		for (auto Key : KeysToRemove)
		{
			LastDamages.Remove(Key);
		}
		
		LastFlushTime = GetWorld()->GetTimeSeconds();
	}
	

}


