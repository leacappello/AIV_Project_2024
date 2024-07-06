// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/SaveGameSubsystem.h"

#include "EngineUtils.h"
#include "AIV_Project/AIV_ProjectCharacter.h"
#include "AIV_Project/Public/Utils.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/MySaveGame.h"
#include "SaveSystem/Savable.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

USaveGameSubsystem::USaveGameSubsystem()
{
	SlotsCount = 5;

	SlotBaseName = "SlotsMetaData";
	SaveGameBaseName = "SaveGame";
	DebugSaveGameName = "DebugSave";
}

void USaveGameSubsystem::LoadSavedGameSlots()
{
	// Load the slots metadata
	if (UGameplayStatics::DoesSaveGameExist(SlotBaseName, 0))
	{
		CurrentSlotSaveGame = Cast<USlotSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotBaseName, 0));
	}
	else
	{
		CurrentSlotSaveGame = Cast<USlotSaveGame>(UGameplayStatics::CreateSaveGameObject(USlotSaveGame::StaticClass()));
		for (int i = 0; i < SlotsCount; i++)
		{
			FString SlotName = SaveGameBaseName;
			SlotName.Append(FString::FromInt(i));
			FSlotData NewSlot = FSlotData();
			NewSlot.SlotName = FName(SlotName);
			NewSlot.SlotDisplayName = FName(SlotName);
			NewSlot.bIsEmpty = true;
			NewSlot.SlotIndex = i;
			CurrentSlotSaveGame->SavedSlots.Add(NewSlot);
		}
		UGameplayStatics::SaveGameToSlot(CurrentSlotSaveGame, SlotBaseName, 0);
	}
}

void USaveGameSubsystem::SaveGame(FString SaveGameName)
{
	if (CurrentSaveGame == nullptr)
	{
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	}
	CurrentSaveGame->SavedActors.Empty();
	// Cycle all actors to be saved
	for (AActor* Actor : ActorsToSave)
	{
		if (!Actor)
			continue;
		
	    FActorSaveData ActorSaveData;
		ActorSaveData.ActorName = Actor->GetFName();
		ActorSaveData.ActorTransform = Actor->GetActorTransform();

		// Start write byte data for all UPROPERTY with IsSaveGame flag
		FMemoryWriter ActorMemoryWriter(ActorSaveData.ActorData);
		FObjectAndNameAsStringProxyArchive ArActor(ActorMemoryWriter, true);
		ArActor.ArIsSaveGame = true;
		Actor->Serialize(ArActor);

		SaveActorComponents(Actor, ActorSaveData);
		CurrentSaveGame->SavedActors.Add(ActorSaveData);

	}

	if (PlayerToSave != nullptr && PlayerToSave->IsValidLowLevel())
	{
		CurrentSaveGame->SavedPlayer.ActorName = PlayerToSave.GetFName();
		CurrentSaveGame->SavedPlayer.ActorTransform = PlayerToSave->GetActorTransform();
		FMemoryWriter MemoryWriter(CurrentSaveGame->SavedPlayer.ActorData);
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		Ar.ArIsSaveGame = true;
		PlayerToSave->Serialize(Ar);
		
		SaveActorComponents(PlayerToSave, CurrentSaveGame->SavedPlayer);
		
		CurrentSaveGame->SavedActors.Add(CurrentSaveGame->SavedPlayer);
	}

	CurrentSaveGameName = SaveGameName != "" ? SaveGameName : DebugSaveGameName;
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, CurrentSaveGameName, 0);
}

void USaveGameSubsystem::SaveGameToCurrentSlot()
{
	SaveGame(CurrentSaveGameName);
}

void USaveGameSubsystem::LoadFromSlot(FString SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		CurrentSaveGameName = SlotName;
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USavable>())
				continue;

			for (FActorSaveData ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					ISavable::Execute_LoadActor(Actor, ActorData);
					LoadActorComponents(Actor, ActorData);
					break;
				}
			}
		}
	}
	else
	{
		LOGERROR("Trying to load invalid or empty slot: %s", *SlotName)
	}
}

void USaveGameSubsystem::ReloadCurrentSlot(const UObject* WorldContextObject)
{
	FSlotData& Slot = CurrentSlotSaveGame->SavedSlots[CurrentSlotIndex];
	
	OpenLevel(WorldContextObject, GetWorld()->GetCurrentLevel(), true, false, Slot);
}

void USaveGameSubsystem::AddActorToSave(AActor* Actor)
{
	ActorsToSave.Add(Actor);
}

void USaveGameSubsystem::OpenLevel(const UObject* WorldContextObject, const TSoftObjectPtr<UWorld> Level,
	bool bAbsolute, bool bNewGame, FSlotData Slot)
{
	FString Options = bNewGame ?
		FString::Printf(TEXT("?NewGame?SlotName=%s"), *Slot.SlotName.ToString()) :
		FString::Printf(TEXT("?SlotName=%s"), *Slot.SlotName.ToString());

	CurrentSlotIndex = Slot.SlotIndex;
	
	if (!bNewGame) // Load
	{
		if (Slot.bIsEmpty)
		{
			LOGERROR("Trying to load game with an empty slot")
			return;
		}
	}
	else // New Game
	{
		FSlotData& SavedSlot = CurrentSlotSaveGame->SavedSlots[CurrentSlotIndex];
		SavedSlot.bIsEmpty = false;
		SavedSlot.CreationDate = FDateTime::UtcNow();
		SavedSlot.SlotDisplayName = Slot.SlotDisplayName;
		SavedSlot.SlotIndex = Slot.SlotIndex;
		UGameplayStatics::SaveGameToSlot(CurrentSlotSaveGame, SlotBaseName, 0);
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(WorldContextObject, Level, bAbsolute, Options);
}

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LOGWARN("SaveGameSubsystem Initialized")
	//ActorsToSave.Empty();

	LoadSavedGameSlots();

}

void USaveGameSubsystem::SaveActorComponents(AActor* Actor, FActorSaveData& ActorSaveData)
{
	TArray<UActorComponent*> ComponentsToSave = Actor->GetComponentsByInterface(USavable::StaticClass());
	for (UActorComponent* Component : ComponentsToSave)
	{
		FComponentData ComponentData;
		ComponentData.ComponentName = Component->GetName();
			
		FMemoryWriter ComponentMemoryWriter(ComponentData.ComponentData);
		FObjectAndNameAsStringProxyArchive ArComp(ComponentMemoryWriter, true);
		ArComp.ArIsSaveGame = true;
		Component->Serialize(ArComp);
		ActorSaveData.SavedComponents.Add(ComponentData);
	}
}

void USaveGameSubsystem::LoadActorComponents(AActor* Actor, const FActorSaveData& ActorSaveData)
{
	TArray<UActorComponent*> SavableComponents = Actor->GetComponentsByInterface(USavable::StaticClass());
	for (FComponentData ComponentData : ActorSaveData.SavedComponents)
	{
		for (UActorComponent* Component : SavableComponents)
		{
			if (Component->GetName() == ComponentData.ComponentName)
			{
				ISavable::Execute_LoadComponent(Component, ComponentData);
				break;
			}
		}
	}
}

