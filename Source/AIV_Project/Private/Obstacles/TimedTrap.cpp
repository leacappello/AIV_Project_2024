// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/TimedTrap.h"

#include "Obstacles/DamageDealerComponent.h"


// Sets default values
ATimedTrap::ATimedTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATimedTrap::BeginPlay()
{
	Super::BeginPlay();
	OnStatusChanged(bStartActive || bAlwaysActive);
	
	if (!bStartActive)
	{
		CooldownTimer = ActivationDelay;
	}

	if (bRandomActivationDelay && !bStartActive && !bAlwaysActive)
	{
		CooldownTimer = FMath::RandRange(0.0f, ActivationDelay);
	}
	
}

void ATimedTrap::OnStatusChanged(bool bNewIsActive)
{
	OnTrapStatusChanged.Broadcast(bNewIsActive);
	DamageDealerComponent->SetActive(bNewIsActive);
}

// Called every frame
void ATimedTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAlwaysActive)
		return;

	if (bIsActive)
	{
		if (DurationTimer > 0)
		{
			DurationTimer -= DeltaTime;
		}
		else
		{
			bIsActive = false;
			OnStatusChanged(bIsActive);
			CooldownTimer = TrapCooldown;
		}
	}
	else
	{
		if (CooldownTimer > 0)
		{
			CooldownTimer -= DeltaTime;
		}
		else
		{
			bIsActive = true;
			OnStatusChanged(bIsActive);
			DurationTimer = TrapDuration;
		}
	}
}

