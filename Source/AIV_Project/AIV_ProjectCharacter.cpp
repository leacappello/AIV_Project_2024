// Copyright Epic Games, Inc. All Rights Reserved.

#include "AIV_ProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/HealthComponent.h"
#include "Interaction/Interactable.h"
#include "Public/Utils.h"
#include "SaveSystem/MySaveGame.h"
#include "SaveSystem/SaveGameSubsystem.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AAIV_ProjectCharacter

AAIV_ProjectCharacter::AAIV_ProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DamageComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("DamageComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	InteractDistance = 1000.f;
	InteractCheckFrequency = 0.1f;
	bDebugInteraction = false;
}

void AAIV_ProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	InteractionTimer -= DeltaSeconds;
	if (InteractionTimer <= 0)
	{
		CheckForInteractables();
		InteractionTimer = InteractCheckFrequency;
	}
}


void AAIV_ProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	InteractionTimer = InteractCheckFrequency;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	ISavable::Execute_Initialize(this);

	if (const UMySaveGame* Data = GetGameInstance()->GetSubsystem<USaveGameSubsystem>()->GetCurrentSaveGame())
	{
		const FActorSaveData& PlayerData = Data->SavedPlayer;
		ISavable::Execute_LoadActor(this, PlayerData);
	}

	DamageComponent->OnDeath.AddUniqueDynamic(this, &AAIV_ProjectCharacter::OnDeath);

}

// ISavable
void AAIV_ProjectCharacter::Initialize_Implementation()
{
	GetGameInstance()->GetSubsystem<USaveGameSubsystem>()->RegisterPlayerToSave(this);
}

void AAIV_ProjectCharacter::LoadActor_Implementation(const FActorSaveData& ActorSaveData)
{
	if (ActorSaveData.ActorName != GetFName())
	{
		LOGERROR("Trying to load the wrong player or data corrupted")
		return;
	}

	SetActorTransform(ActorSaveData.ActorTransform);

	FMemoryReader MemReader(ActorSaveData.ActorData);
	FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
	Ar.ArIsSaveGame = true;
	// Convert binary array back into actor's variables
	Serialize(Ar);

	GetGameInstance()->GetSubsystem<USaveGameSubsystem>()->LoadActorComponents(this, ActorSaveData);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAIV_ProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAIV_ProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAIV_ProjectCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AAIV_ProjectCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAIV_ProjectCharacter::OnDeath(UObject* InInstigator)
{
	LOGINFO("Killed by %s", *InInstigator->GetName())
	USaveGameSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameSubsystem>();
	SaveSubsystem->ReloadCurrentSlot(this);
}

void AAIV_ProjectCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAIV_ProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAIV_ProjectCharacter::Interact()
{
	if (CurrentInteractable)
	{
		IInteractable::Execute_Interact(CurrentInteractable.GetObject(), this);
	}
	
}

void AAIV_ProjectCharacter::CheckForInteractables()
{
	FHitResult HitResult;
	const FName TraceTag("InteractTrace");

	FVector Start = FollowCamera->GetComponentLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * 1000;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (bDebugInteraction)
	{
		GetWorld()->DebugDrawTraceTag = TraceTag;
		CollisionParams.TraceTag = TraceTag;
	}
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParams);

	if (AActor* HitActor = HitResult.GetActor())
	{
		if (CurrentInteractable == HitActor)
		{
			if (!IInteractable::Execute_CanInteract(HitActor))
			{
				IInteractable::Execute_SetOnInteractionRange(CurrentInteractable.GetObject(), false);
				CurrentInteractable = nullptr;
			}
			
			return;
		}

		if (CurrentInteractable)
		{
			IInteractable::Execute_SetOnInteractionRange(CurrentInteractable.GetObject(), false);
			CurrentInteractable = nullptr;
		}
		
		if (HitActor->Implements<UInteractable>() && IInteractable::Execute_CanInteract(HitActor))
		{
			CurrentInteractable = TScriptInterface<IInteractable>(HitActor);
			IInteractable::Execute_SetOnInteractionRange(CurrentInteractable.GetObject(), true);
		}
	}
	else if (CurrentInteractable)
	{
		IInteractable::Execute_SetOnInteractionRange(CurrentInteractable.GetObject(), false);
		CurrentInteractable = nullptr;
	}
}


