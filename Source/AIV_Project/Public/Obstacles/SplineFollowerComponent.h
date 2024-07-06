// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "SplineFollowerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIV_PROJECT_API USplineFollowerComponent : public USplineComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USplineFollowerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bLoop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "bLoop", EditConditionHides))
	bool bPingPong;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnComponentCreated() override;

	float CurrentDistance;

	bool bMoving;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
