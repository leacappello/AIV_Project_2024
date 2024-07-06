// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/Savable.h"
#include "Door.generated.h"

class ABaseInteractable;

UCLASS()
class AIV_PROJECT_API ADoor : public AActor, public ISavable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TObjectPtr<ABaseInteractable> OpenInstigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float OpenTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FTransform OpenTransformOffset;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bIsOpen;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bInitialized;

protected:

	bool bIsOpening;

	float CurrentOpenTime;

	UPROPERTY(SaveGame)
	FRotator StartRotation;

	UPROPERTY(SaveGame)
	FTransform StartTransform;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OpenDoor(UObject* InInstigator);

	void UpdateDoorRotation(float DeltaTime);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
