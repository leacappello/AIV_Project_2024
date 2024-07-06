// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/Projectile.h"

#include "NiagaraComponent.h"
#include "Components/HealthComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CollisionComponent);

	TrailEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(CollisionComponent);

	Speed = 10.f;
	Damage = 10.f;
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnImpact);
}

void AProjectile::OnImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (UHealthComponent* HealthComponent = Cast<UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass())))
	{
		HealthComponent->Damage(this, Damage);
	}

	UNiagaraComponent* ImpactEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, GetActorLocation(), GetActorForwardVector().Rotation());
	ImpactEffectComponent->SetAutoDestroy(true);
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation, true);
}

void AProjectile::InitProjectile(const FVector& Direction, AActor* InOwner)
{
	ProjectileOwner = InOwner;
	SetActorRotation(Direction.Rotation());
}



