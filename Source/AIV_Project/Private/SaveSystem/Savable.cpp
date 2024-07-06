// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/Savable.h"

#include "AIV_Project/Public/Utils.h"
#include "SaveSystem/MySaveGame.h"
#include "SaveSystem/SaveGameSubsystem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


// Add default functionality here for any ISavable functions that are not pure virtual.
void ISavable::Initialize_Implementation()
{

	AActor* Actor = Cast<AActor>(this);
	if (Actor == nullptr)
		return;
	
	UGameInstance* GameInstance = Actor->GetWorld()->GetGameInstance();
	USaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<USaveGameSubsystem>();
	SaveGameSubsystem->AddActorToSave(Actor);

}

void ISavable::LoadActor_Implementation(const FActorSaveData& ActorSaveData)
{
	AActor* Actor = Cast<AActor>(this);

	if (Actor == nullptr || Actor->GetFName() != ActorSaveData.ActorName)
	{
		LOGERROR("Trying to load data into the wrong actor")
		return;
	}

	Actor->SetActorTransform(ActorSaveData.ActorTransform);

	FMemoryReader MemReader(ActorSaveData.ActorData);

	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Actor->Serialize(Ar);
}

void ISavable::LoadComponent_Implementation(const FComponentData& ComponentSaveData)
{
	UActorComponent* Component = Cast<UActorComponent>(this);

	if (Component == nullptr || Component->GetName() != ComponentSaveData.ComponentName)
	{
		LOGERROR("Trying to load wrong component")
		return;
	}

	FMemoryReader MemReader(ComponentSaveData.ComponentData);

	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Component->Serialize(Ar);
}

