// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/DamageDealerComponent.h"

#include "AIV_Project/AIV_ProjectCharacter.h"
#include "AIV_Project/Public/Utils.h"
#include "Components/HealthComponent.h"
#include "Components/ShapeComponent.h"


// Sets default values for this component's properties
UDamageDealerComponent::UDamageDealerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UDamageDealerComponent::OnAreaOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FDamageArea* DamageArea = GetAssociatedDamageArea(Cast<UShapeComponent>(OverlappedComponent));
	if (DamageArea->bDamageOnlyPlayer && !OtherActor->IsA(AAIV_ProjectCharacter::StaticClass()))
		return;
		
	if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		DamageArea->DamageComponents.AddUnique(HealthComponent);
	}

	LOGINFO("Actor enter on damage area %s", *OtherActor->GetName())

}

void UDamageDealerComponent::OnAreaOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	FDamageArea* DamageArea = GetAssociatedDamageArea(Cast<UShapeComponent>(OverlappedComponent));
	if (DamageArea->bDamageOnlyPlayer && !OtherActor->IsA(AAIV_ProjectCharacter::StaticClass()))
		return;
	
	if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
		DamageArea->DamageComponents.Remove(HealthComponent);

	LOGINFO("Actor exit from damage area %s", *OtherActor->GetName())
}

FDamageArea* UDamageDealerComponent::GetAssociatedDamageArea(UShapeComponent* ShapeComponent)
{
	for (FDamageArea& Element : DamageAreas)
	{
		if (Element.Area == ShapeComponent)
		{
			return &Element;
		}
	}
	return nullptr;
}

void UDamageDealerComponent::OnAreaDeactivated(UActorComponent* Component)
{
	if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Component))
	{
		FDamageArea* DamageArea = GetAssociatedDamageArea(ShapeComponent);
		if (DamageArea)
		{
			DamageArea->bAreaActive = false;
		}
	}
}

void UDamageDealerComponent::OnAreaActivated(UActorComponent* Component, bool bReset)
{
	if (UShapeComponent* ShapeComponent = Cast<UShapeComponent>(Component))
	{
		FDamageArea* DamageArea = GetAssociatedDamageArea(ShapeComponent);
		if (DamageArea)
		{
			DamageArea->bAreaActive = true;
		}
	}
}

// Called when the game starts
void UDamageDealerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	for (FDamageArea& DamageArea : DamageAreas)
	{
		for (auto Component : GetOwner()->GetComponents())
		{
			if (Component->GetName() == DamageArea.ShapeName)
			{
				DamageArea.Area = Cast<UShapeComponent>(Component);
				break;
			}
		}
		if (DamageArea.Area)
		{
			DamageArea.Area->OnComponentBeginOverlap.AddUniqueDynamic(this, &UDamageDealerComponent::OnAreaOverlapBegin);
			DamageArea.Area->OnComponentEndOverlap.AddUniqueDynamic(this, &UDamageDealerComponent::OnAreaOverlapEnd);
			DamageArea.Area->OnComponentActivated.AddUniqueDynamic(this, &UDamageDealerComponent::OnAreaActivated);
			DamageArea.Area->OnComponentDeactivated.AddUniqueDynamic(this, &UDamageDealerComponent::OnAreaDeactivated);
		}
	}
}


// Called every frame
void UDamageDealerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	for (FDamageArea& DamageArea : DamageAreas)
	{
		if (DamageArea.DamageComponents.IsEmpty() || !DamageArea.bAreaActive)
			continue;

		// Reverse loop to avoid modifying the array while iterating if the component is destroyed
		for (int i = DamageArea.DamageComponents.Num() - 1; i >= 0; --i)
		{
			UHealthComponent* HealthComponent = DamageArea.DamageComponents[i];
			if (DamageArea.bContinuousDamage)
			{
				HealthComponent->Damage(DamageArea.Area, DeltaTime * DamageArea.DamageRate);
				LOGWARN("Continous Damage: %f to %s", DeltaTime * DamageArea.DamageRate, *HealthComponent->GetOwner()->GetName())
			}
			else if (HealthComponent->GetLastDamageTime(DamageArea.Area) + DamageArea.DamageDelay < GetWorld()->GetTimeSeconds())
			{
				HealthComponent->Damage(DamageArea.Area, DamageArea.Damage);
				LOGWARN("Damage: %f to %s",  DamageArea.Damage, *HealthComponent->GetOwner()->GetName())
			}
		}
	}
}

