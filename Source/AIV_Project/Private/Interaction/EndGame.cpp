// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/EndGame.h"

#include "MyGameMode.h"
#include "AIV_Project/AIV_ProjectGameMode.h"


// Sets default values
AEndGame::AEndGame()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEndGame::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void AEndGame::OnInteraction(AActor* Caller)
{
	Super::OnInteraction(Caller);
	// Get Game mode
	if (const AMyGameMode* GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnEndGame.Broadcast();
	}
}

// Called every frame
void AEndGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

