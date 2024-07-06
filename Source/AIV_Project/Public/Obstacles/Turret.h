// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class AProjectile;

UCLASS()
class AIV_PROJECT_API ATurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurret();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> TurretHeadMesh;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float FireRate;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float FireRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float ShootAngleRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FName FireSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float SearchTargetInterval;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SearchTarget();

	void RotateTowardTarget(float DeltaTime);

	bool CanSeeTarget(AActor* Target) const;

	void Shoot();

	float FireTimer;

	float SearchTargetTimer;

	bool bCanFire;

	UPROPERTY()
	AActor* CurrentTarget;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
