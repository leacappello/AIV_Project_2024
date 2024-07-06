// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/Door.h"

#include "Interaction/BaseInteractable.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsOpen = false;
	bInitialized = false;
	OpenTime = 1;
	OpenTransformOffset.SetScale3D(FVector::ZeroVector);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	ISavable::Execute_Initialize(this);

	OpenInstigator.Get()->OnInteractionEvent.AddUniqueDynamic(this, &ADoor::OpenDoor);

	if (bInitialized)
		return;
	
	StartRotation = GetActorRotation();
	StartTransform = GetActorTransform();
	bInitialized = true;
	
}

void ADoor::OpenDoor(UObject* InInstigator)
{
	bIsOpening = true;
	CurrentOpenTime = 0;
}

void ADoor::UpdateDoorRotation(float DeltaTime)
{
	if (bIsOpening)
	{
		CurrentOpenTime += DeltaTime;
		FTransform TargetTransform;
		TargetTransform.SetLocation(StartTransform.GetLocation() + OpenTransformOffset.GetLocation());
		TargetTransform.SetRotation(FQuat(StartTransform.GetRotation()) * FQuat(OpenTransformOffset.GetRotation()));
		TargetTransform.SetScale3D(StartTransform.GetScale3D() + OpenTransformOffset.GetScale3D());
		FTransform NewTransform = UKismetMathLibrary::TLerp(StartTransform, TargetTransform, CurrentOpenTime);
		SetActorTransform(NewTransform);
		if (CurrentOpenTime >= OpenTime)
		{
			bIsOpening = false;
			bIsOpen = true;
		}
	}	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateDoorRotation(DeltaTime);
}

