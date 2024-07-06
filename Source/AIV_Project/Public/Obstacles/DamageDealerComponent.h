// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor.h"
#include "Components/ActorComponent.h"
#include "Components/ShapeComponent.h"
#include "DamageDealerComponent.generated.h"

class UHealthComponent;

USTRUCT(BlueprintType)
struct FDamageArea
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString ShapeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bContinuousDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bDamageOnlyPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "bContinuousDamage", EditConditionHides))
	float DamageRate; // Damage per second

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "!bContinuousDamage", EditConditionHides))
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "!bContinuousDamage", EditConditionHides))
	float DamageDelay; // Time between damage
	
	UPROPERTY()
	TArray<UHealthComponent*> DamageComponents;

	
	bool bAreaActive;

	TObjectPtr<UShapeComponent> Area;
	FDamageArea()
	{
		Area = nullptr;
		bContinuousDamage = false;
		bDamageOnlyPlayer = false;
		DamageRate = 0.0f;
		Damage = 0;
		DamageDelay = 0.0f;
		bAreaActive = true;
	}

};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AIV_PROJECT_API UDamageDealerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDamageDealerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<FDamageArea> DamageAreas;

protected:
	UFUNCTION()
	void OnAreaOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAreaOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FDamageArea* GetAssociatedDamageArea(UShapeComponent* ShapeComponent);

	UFUNCTION()
	void OnAreaDeactivated(UActorComponent* Component);
	
	UFUNCTION()
	void OnAreaActivated(UActorComponent* Component, bool bReset);
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
