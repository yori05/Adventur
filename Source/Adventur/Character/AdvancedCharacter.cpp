// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Adventur/Components/LockerComponent.h"
#include "Adventur/Components/TargetableComponent.h"
#include "Adventur/Components/AdvancedMovementComponent.h"
#include "Adventur/Components/InventoryComponent.h"
#include "Adventur/Interfaces/InteractibleInterface.h"

// Sets default values
AAdvancedCharacter::AAdvancedCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UAdvancedMovementComponent>(AAdvancedCharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	AdvancedMovementComponent = Cast<UAdvancedMovementComponent>(GetCharacterMovement());
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	DetectionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectorComponent"));
	DetectionComponent->SetupAttachment(GetMesh());

	LockerComponent = CreateDefaultSubobject<ULockerComponent>(TEXT("LockerComponent"));
	LockerComponent->SetDetectionComponent(DetectionComponent);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void AAdvancedCharacter::BeginPlay()
{
	Super::BeginPlay();
	AdvancedMovementComponent = Cast<UAdvancedMovementComponent>(GetCharacterMovement());
}

// Called every frame
void AAdvancedCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdvancedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAdvancedCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAdvancedCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AAdvancedCharacter::SidesStep(const FVector& MovementDirection)
{
	if (IsValid(AdvancedMovementComponent))
	AdvancedMovementComponent->SidesStep(MovementDirection);
}

void AAdvancedCharacter::LockTheTarget()
{
	if (IsValid(LockerComponent))
	{
		LockerComponent->LockGreatestTarget();
	}
}

void AAdvancedCharacter::UnlockTheTarget()
{
	if (IsValid(LockerComponent))
	{
		LockerComponent->UnlockTarget();
	}
}

bool AAdvancedCharacter::HasATarget()
{
	if (IsValid(LockerComponent))
	{
		return LockerComponent->HasATarget();
	}

	return false;
}

UTargetableComponent* AAdvancedCharacter::GetCurrentTarget()
{
	if (HasATarget())
	{
		return LockerComponent->GetCurrentTarget();
	}

	return nullptr;
}

void AAdvancedCharacter::ChangeAutoLock(bool NewAutoLock)
{
	if (IsValid(LockerComponent))
	{
		LockerComponent->ChangeAutoLock(NewAutoLock);
	}

}

void AAdvancedCharacter::ChangeState(ECharacterState NewState)
{

}

ECharacterState AAdvancedCharacter::GetState()
{
	return CharacterState;
}

void AAdvancedCharacter::Interact()
{
	IInteractibleInterface* Interactible = nullptr;

	if (HasATarget())
	{
		auto CurrentTarget = GetCurrentTarget();

		if (IsValid(CurrentTarget))
		{
			Interactible = Cast<IInteractibleInterface>(CurrentTarget->GetOwner());
		}
	}

	if (Interactible != nullptr)
	{
		Interactible->OnBeginInteract(this);
	}
}

//void AAdvancedCharacter::LaunchALink_Implementation(APortal* NewPortalEnter, APortal* NewPortalExit){}
//bool AAdvancedCharacter::IsLinked_Implementation(APortal* Portal){}
//void AAdvancedCharacter::SwapPosition_Implementation(FVector RelativePosition, FQuat RelativeRotation){}
//void AAdvancedCharacter::BreakLink_Implementation(){}


void AAdvancedCharacter::LaunchTeleport_Implementation(AActor* ToTeleport, FVector RelativePosition)
{
	if (ToTeleport != nullptr && !bIsRunning)
	{
		auto Destination = ToTeleport->GetActorLocation();
		//Destination += GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
		Destination += RelativePosition;
		SetActorLocation(Destination, false, nullptr, ETeleportType::TeleportPhysics);
		bIsRunning = true;
	}
}

void AAdvancedCharacter::ReleaseTeleport_Implementation()
{
	bIsRunning = false;
}

bool AAdvancedCharacter::IsTeleportationRunning_Implementation()
{
	return bIsRunning;
}

