// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSlotData
{
	GENERATED_BODY()

public:
	FSlotData()
	{
		SlotName = "Empty Slot";
		CreationDate = FDateTime::Now();
		SlotDisplayName = "Empty Slot";
		bIsEmpty = true;
		SlotIndex = 0;
	}
	
	UPROPERTY(BlueprintReadWrite)
	FName SlotName;

	UPROPERTY(BlueprintReadWrite)
	FDateTime CreationDate;

	UPROPERTY(BlueprintReadWrite)
	FName SlotDisplayName;

	UPROPERTY(BlueprintReadWrite)
	bool bIsEmpty;

	UPROPERTY(BlueprintReadWrite)
	int SlotIndex;
};

USTRUCT(BlueprintType)
struct FComponentData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString ComponentName;

	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> ComponentData;	
	
};

USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()

public:
	FActorSaveData()
	{
		ActorName = "Empty Actor";
		ActorTransform = FTransform();
		ActorData = TArray<uint8>();
	}
	UPROPERTY(BlueprintReadWrite)
	FName ActorName;

	UPROPERTY(BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(BlueprintReadWrite)
	TArray<FComponentData> SavedComponents;

	UPROPERTY(BlueprintReadWrite)
	TArray<uint8> ActorData;
	
};

UCLASS()
class USlotSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSlotData> SavedSlots;
	
};

UCLASS()
class UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FActorSaveData> SavedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FActorSaveData SavedPlayer;

};
