// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "EndGame.generated.h"

UCLASS()
class AIV_PROJECT_API AEndGame : public ABaseInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEndGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteraction(AActor* Caller) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
