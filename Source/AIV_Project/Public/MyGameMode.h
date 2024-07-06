// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndGame);


/**
 * 
 */
UCLASS()
class AIV_PROJECT_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
public:	
	UPROPERTY(BlueprintAssignable)
	FOnEndGame OnEndGame;

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
