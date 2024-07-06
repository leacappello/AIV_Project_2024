// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/SplineFollowerComponent.h"


// Sets default values for this component's properties
USplineFollowerComponent::USplineFollowerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bLoop = true;
	bPingPong = false;
	Speed = 100;
	CurrentDistance = 0;
	SetAbsolute(true, true, true);
	// ...
}


// Called when the game starts
void USplineFollowerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get the closest point on the spline to the actor's location
	FVector ClosestPoint = FindLocationClosestToWorldLocation(GetOwner()->GetActorLocation(), ESplineCoordinateSpace::World);
	CurrentDistance = GetDistanceAlongSplineAtLocation(ClosestPoint, ESplineCoordinateSpace::World);
	GetOwner()->SetActorLocation(ClosestPoint);

	bMoving = true;
}

void USplineFollowerComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	SetWorldLocation(GetOwner()->GetActorLocation());
}


// Called every frame
void USplineFollowerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bMoving)
		return;
	
	CurrentDistance += Speed * DeltaTime;
	if (CurrentDistance > GetSplineLength() || CurrentDistance < 0)
	{
		if (bLoop)
		{
			if (bPingPong)
				Speed *= -1;
			else
				CurrentDistance = CurrentDistance > GetSplineLength() ? 0 : GetSplineLength();
		}
		else
		{
			bMoving = false;
		}
	}

	FVector Location = GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	GetOwner()->SetActorLocation(Location);
}

