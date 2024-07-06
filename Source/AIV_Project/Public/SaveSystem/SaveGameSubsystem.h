// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

struct FActorSaveData;
struct FComponentData;
class AAIV_ProjectCharacter;
class UMySaveGame;
class USlotSaveGame;
struct FSlotData;
/**
 * 
 */
UCLASS(meta = (DisplayName = "SaveGame Subsystem"))
class AIV_PROJECT_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	USaveGameSubsystem();

protected:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorsToSave;

	TObjectPtr<AAIV_ProjectCharacter> PlayerToSave;

	UPROPERTY()
	TArray<FSlotData> Slots;

	int SlotsCount;

	FString SlotBaseName;

	FString SaveGameBaseName;
	FString DebugSaveGameName;
	
	// Contains the slot meta data
	UPROPERTY()
	USlotSaveGame* CurrentSlotSaveGame;

	// Contains the actual saved game data
	UPROPERTY()
	UMySaveGame* CurrentSaveGame;

	FString CurrentSaveGameName;

	int CurrentSlotIndex;

	void LoadSavedGameSlots();

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveGame(FString SaveGameName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void SaveGameToCurrentSlot();

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void LoadFromSlot(FString SlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void ReloadCurrentSlot(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void AddActorToSave(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	const USlotSaveGame* GetCurrentSlotSaveGame() const { return CurrentSlotSaveGame; }

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	const UMySaveGame* GetCurrentSaveGame() const {return CurrentSaveGame; }

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void RegisterPlayerToSave(AAIV_ProjectCharacter* Player) {PlayerToSave = Player; }

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void OpenLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level, bool bAbsolute, bool bNewGame, FSlotData Slot);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SaveActorComponents(AActor* Actor, FActorSaveData& ActorSaveData);
	
	void LoadActorComponents(AActor* Actor, const FActorSaveData& ActorSaveData);

};
