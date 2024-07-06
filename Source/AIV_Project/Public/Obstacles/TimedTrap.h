// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTrap.h"
#include "TimedTrap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrapStatusChanged, bool, bIsActive);


UCLASS()
class AIV_PROJECT_API ATimedTrap : public ABaseTrap
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimedTrap();

	UPROPERTY(BlueprintAssignable)
	FOnTrapStatusChanged OnTrapStatusChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bAlwaysActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bStartActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float TrapDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float TrapCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config" , meta = (EditCondition = "!bAlwaysActive && !bStartActive"))
	bool bRandomActivationDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (ClampMin = 0.0f, UIMin = 0.0f, EditCondition = "!bStartActive"))
	float ActivationDelay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnStatusChanged(bool bNewIsActive);

	float CooldownTimer;

	float DurationTimer;

	bool bIsActive;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsTrapActive() const { return bIsActive; }

};
