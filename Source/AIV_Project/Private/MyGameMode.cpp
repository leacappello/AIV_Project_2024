// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Kismet/GameplayStatics.h"

#include "SaveSystem/SaveGameSubsystem.h"

void AMyGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options,"SlotName");
	bool bNewGame = UGameplayStatics::HasOption(Options, "NewGame");
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	USaveGameSubsystem* SaveGameSubsystem = GameInstance->GetSubsystem<USaveGameSubsystem>();
	if (bNewGame)
	{
		SaveGameSubsystem->SaveGame(SelectedSaveSlot);
	}
	else // Load
	{

		SaveGameSubsystem->LoadFromSlot(SelectedSaveSlot);
	}
}
