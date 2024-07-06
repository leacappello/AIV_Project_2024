// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/SpikeTrap.h"

#include "Components/BoxComponent.h"
#include "Obstacles/DamageDealerComponent.h"


// Sets default values
ASpikeTrap::ASpikeTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpikeDamageArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpikeDamageArea"));
	SpikeDamageArea->SetupAttachment(RootComponent);

	FDamageArea DamageArea;
	DamageArea.Damage = 30;
	DamageArea.bContinuousDamage = false;
	DamageArea.DamageDelay = 2.f;
	DamageArea.ShapeName = SpikeDamageArea.GetName();
	
	DamageDealerComponent.Get()->DamageAreas.Add(DamageArea);
}

// Called when the game starts or when spawned
void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	AnimationStartPosition = GetActorLocation();
	AnimationEndPosition = AnimationStartPosition + AnimationEndOffset;
}

void ASpikeTrap::OnStatusChanged(bool bNewIsActive)
{
	Super::OnStatusChanged(bNewIsActive);
	bSpikeMoving = true;
	bSpikeGoingToEnd = bNewIsActive;
}

void ASpikeTrap::MoveSpike(float DeltaTime)
{
	FVector CurrentPosition = GetActorLocation();
	FVector TargetPosition = bSpikeGoingToEnd ? AnimationEndPosition : AnimationStartPosition;
	FVector NewPosition = FMath::VInterpTo(CurrentPosition, TargetPosition, DeltaTime, SpikeSpeed);
	SetActorLocation(NewPosition);
	if (NewPosition.Equals(TargetPosition, 0.1f))
	{
		bSpikeMoving = false;
	}
}

// Called every frame
void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSpikeMoving)
		return;

	MoveSpike(DeltaTime);
}


