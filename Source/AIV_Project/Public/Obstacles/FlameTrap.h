// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimedTrap.h"
#include "NiagaraComponent.h"
#include "FlameTrap.generated.h"

UCLASS()
class AIV_PROJECT_API AFlameTrap : public ATimedTrap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlameTrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraComponent> FlameEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnStatusChanged(bool bNewIsActive) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
