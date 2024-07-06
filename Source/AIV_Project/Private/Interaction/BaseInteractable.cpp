// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseInteractable.h"

#include "Components/TextRenderComponent.h"


// Sets default values
ABaseInteractable::ABaseInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanInteract = true;
	bIsInInteractionRange = false;
	bAllowMultipleInteractions = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(false);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(Mesh);
	TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRender->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	TextRender->SetRelativeLocation(FVector(0, 0, 100));
	TextRender->Text = FText::FromString("Press F to interact");
}

void ABaseInteractable::Interact_Implementation(AActor* Caller)
{
	OnInteraction(Caller);
}

bool ABaseInteractable::CanInteract_Implementation()
{
	return bCanInteract;
}

void ABaseInteractable::SetOnInteractionRange_Implementation(bool bIsInRange)
{
	TextRender->SetVisibility(bIsInRange && bCanInteract);
}

// Called when the game starts or when spawned
void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();

	ISavable::Execute_Initialize(this);
	
	TextRender->SetVisibility(false);
	
}

void ABaseInteractable::OnInteraction(AActor* Caller)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted"))
	OnInteractionEvent.Broadcast(Caller);
	if (!bAllowMultipleInteractions)
	{
		bCanInteract = false;
		TextRender->SetVisibility(false);

		if (bDestroyOnInteract)
			Destroy();		
	}
}

void ABaseInteractable::RefreshStatus()
{
	if (!bCanInteract)
	{
		if (bDestroyOnInteract)
			Destroy();
	}
}

// Called every frame
void ABaseInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bTextFaceCamera)
		return;

	FVector CameraPosition = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector DirectionToCamera = CameraPosition - GetActorLocation();
	DirectionToCamera.Normalize();
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToCamera).Rotator();
	TextRender->SetWorldRotation(LookAtRotation);
}

void ABaseInteractable::LoadActor_Implementation(const FActorSaveData& ActorSaveData)
{
	ISavable::LoadActor_Implementation(ActorSaveData);

	RefreshStatus();
}

