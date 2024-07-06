// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacles/Turret.h"

#include "AIV_Project/AIV_ProjectCharacter.h"
#include "AIV_Project/Public/Utils.h"
#include "Kismet/GameplayStatics.h"
#include "Obstacles/Projectile.h"


// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	RootComponent = TurretMesh;

	TurretHeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHeadMesh"));
	TurretHeadMesh->SetupAttachment(TurretMesh);
	

	FireRate = 1.f;
	FireRange = 1000.f;
	ShootAngleRange = 10.f;
	TurnRate = 180.f;
	SearchTargetInterval = 0.5f;
	FireSocketName = "FireSocket";
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATurret::SearchTarget()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn)};
	TArray<AActor*> OverlappedActors;
	UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), FireRange, ObjectTypes, AAIV_ProjectCharacter::StaticClass(), {this}, OverlappedActors);
	if (!OverlappedActors.IsEmpty() && OverlappedActors[0] && CanSeeTarget(OverlappedActors[0])
		&& OverlappedActors[0]->IsA(AAIV_ProjectCharacter::StaticClass()))
	{
		CurrentTarget = OverlappedActors[0];
	}
	else
	{
		CurrentTarget = nullptr;
	}
}

void ATurret::RotateTowardTarget(float DeltaTime)
{

	FVector TargetDirection = CurrentTarget->GetActorLocation() - TurretHeadMesh->GetComponentLocation();
	TargetDirection.Normalize();

	FRotator TargetRotation = TargetDirection.Rotation();
	FRotator CurrentRotation = TurretHeadMesh->GetComponentRotation();

	FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, TurnRate);
	TurretHeadMesh->SetWorldRotation(NewRotation);

	// Get angle between turret head forward vector and target direction
	float Angle = FMath::Acos(FVector::DotProduct(TurretHeadMesh->GetForwardVector(), TargetDirection)) * 180.f / PI;
	bCanFire = Angle <= ShootAngleRange;
}

bool ATurret::CanSeeTarget(AActor* Target) const
{
	bool bVisible = false;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	GetWorld()->LineTraceSingleByChannel(HitResult, TurretHeadMesh->GetComponentLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_Visibility, CollisionParams);
	if (HitResult.GetActor() == Target)
	{
		bVisible = true;
	}
	else if (HitResult.GetActor())
	{
		LOGWARN("hitted actor: %s", *HitResult.GetActor()->GetName())
	}
	return bVisible;
}

void ATurret::Shoot()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, TurretHeadMesh->GetSocketLocation(FireSocketName), TurretHeadMesh->GetSocketRotation(FireSocketName));
	Projectile->InitProjectile(TurretHeadMesh->GetForwardVector(), this);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SearchTargetTimer -= DeltaTime;
	if (SearchTargetTimer <= 0.f)
	{
		SearchTargetTimer = SearchTargetInterval;
		SearchTarget();
	}
	
	if (!CurrentTarget)
		return;

	RotateTowardTarget(DeltaTime);

	if (bCanFire)
	{
		FireTimer -= DeltaTime;
		if (FireTimer <= 0.f)
		{
			FireTimer = FireRate;
			Shoot();
		}
	}
}

