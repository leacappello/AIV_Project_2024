// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/SavePoint.h"

#include "NiagaraComponent.h"
#include "Components/TextRenderComponent.h"
#include "SaveSystem/SaveGameSubsystem.h"

// Sets default values
ASavePoint::ASavePoint() : ABaseInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActivatedEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ActivatedEffect"));
	ActivatedEffect->SetupAttachment(RootComponent);
	
	TextRender->Text = FText::FromString("Press F to save");
	bCanInteract = true;
}

// Called when the game starts or when spawned
void ASavePoint::BeginPlay()
{
	Super::BeginPlay();
	ActivatedEffect->SetActive(!bCanInteract);

}

void ASavePoint::OnInteraction(AActor* Caller)
{
	Super::OnInteraction(Caller);

	bCanInteract = false;
	ActivatedEffect->SetActive(true);
	GetGameInstance()->GetSubsystem<USaveGameSubsystem>()->SaveGameToCurrentSlot();
}

void ASavePoint::RefreshStatus()
{
	Super::RefreshStatus();
	ActivatedEffect->SetVisibility(!bCanInteract);
}



