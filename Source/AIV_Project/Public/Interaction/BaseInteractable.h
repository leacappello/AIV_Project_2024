// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/Savable.h"
#include "BaseInteractable.generated.h"

class UTextRenderComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, UObject*, Instigator);

UCLASS()
class AIV_PROJECT_API ABaseInteractable : public AActor, public IInteractable, public ISavable
{
	GENERATED_BODY()

public:
	ABaseInteractable();

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UTextRenderComponent> TextRender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bTextFaceCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bAllowMultipleInteractions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "!bAllowMultipleInteractions"))
	bool bDestroyOnInteract;
	
	void Interact_Implementation(AActor* Caller) override;

	bool CanInteract_Implementation() override;

	virtual void SetOnInteractionRange_Implementation(bool bIsInRange) override;

	UPROPERTY(BlueprintAssignable)
	FOnInteraction OnInteractionEvent;
	
protected:

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bCanInteract;

	bool bIsInInteractionRange;
	
	virtual void BeginPlay() override;

	virtual void OnInteraction(AActor* Caller);

	virtual void RefreshStatus();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LoadActor_Implementation(const FActorSaveData& ActorSaveData) override;
};
