// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/BaseInteractable.h"
#include "SavePoint.generated.h"

class UNiagaraComponent;

UCLASS()
class AIV_PROJECT_API ASavePoint : public ABaseInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASavePoint();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UNiagaraComponent> ActivatedEffect;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteraction(AActor* Caller) override;

	virtual void RefreshStatus() override;

public:


};
