// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MySaveGame.h"
#include "UObject/Interface.h"
#include "Savable.generated.h"

struct FComponentData;
struct FActorSaveData;
// This class does not need to be modified.
UINTERFACE()
class USavable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AIV_PROJECT_API ISavable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="SaveSystem")
	void Initialize();

	virtual void Initialize_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="SaveSystem")
	void LoadActor(const FActorSaveData& ActorSaveData);

	virtual void LoadActor_Implementation(const FActorSaveData& ActorSaveData);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="SaveSystem")
	void LoadComponent(const FComponentData& ComponentSaveData);

	virtual void LoadComponent_Implementation(const FComponentData& ComponentSaveData);
};
