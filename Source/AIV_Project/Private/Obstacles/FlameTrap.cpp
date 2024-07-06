// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/FlameTrap.h"


// Sets default values
AFlameTrap::AFlameTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlameEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlameEffect"));
	FlameEffect->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlameTrap::BeginPlay()
{
	Super::BeginPlay();
}

void AFlameTrap::OnStatusChanged(bool bNewIsActive)
{
	Super::OnStatusChanged(bNewIsActive);
	if (bNewIsActive)
	{
		FlameEffect->Activate();
	}
	else
	{
		FlameEffect->Deactivate();
	}
}

// Called every frame
void AFlameTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

