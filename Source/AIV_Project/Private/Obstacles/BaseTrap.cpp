// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/BaseTrap.h"

#include "Obstacles/DamageDealerComponent.h"


// Sets default values
ABaseTrap::ABaseTrap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	
	DamageDealerComponent = CreateDefaultSubobject<UDamageDealerComponent>(TEXT("DamageDealerComponent"));
}

// Called when the game starts or when spawned
void ABaseTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

