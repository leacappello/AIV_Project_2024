// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimedTrap.h"
#include "SpikeTrap.generated.h"

class UBoxComponent;

UCLASS()
class AIV_PROJECT_API ASpikeTrap : public ATimedTrap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpikeTrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UBoxComponent> SpikeDamageArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FVector AnimationEndOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float SpikeSpeed;

protected:
	FVector AnimationStartPosition;

	FVector AnimationEndPosition;

	bool bSpikeMoving;

	bool bSpikeGoingToEnd;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnStatusChanged(bool bNewIsActive) override;

	void MoveSpike(float DeltaTime);
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
