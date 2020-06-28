// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedMovementComponent.h"
#include "GameFramework/Character.h"

uint8 UAdvancedMovementComponent::GetCustomMovementMode() const
{
	return CustomMovementMode;
}

EAdvancedMovementMode UAdvancedMovementComponent::GetAdvancedMovementMode() const
{
	return (EAdvancedMovementMode)CustomMovementMode;
}


void UAdvancedMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (GetAdvancedMovementMode())
	{
	case EAdvancedMovementMode::ADV_MOVE_Dodge :
		PhysDodge(deltaTime, Iterations);
		break;
	case EAdvancedMovementMode::ADV_MOVE_SidesStep :
		PhysSidesStep(deltaTime, Iterations);
		break;
	case EAdvancedMovementMode::ADV_MOVE_None :
		break;
	case EAdvancedMovementMode::ADV_MOVE_Max :
		break;
	default :
		break;
	}
}

/** ------------------- Dodge Part Begin ---------------------------------- */
bool UAdvancedMovementComponent::CanDodge() const
{
	return MovementMode == EMovementMode::MOVE_Walking;
}

void UAdvancedMovementComponent::Dodge(const FVector& NewDodgeDirection)
{
	if (!CanDodge())
		return;

	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EAdvancedMovementMode::ADV_MOVE_Dodge);
	bOldOrientRotationToMovement = bOrientRotationToMovement;
	bOrientRotationToMovement = false;

	DodgeDirection = (NewDodgeDirection.IsNearlyZero()) ? CharacterOwner->GetActorForwardVector() : NewDodgeDirection;
	DodgeDistanceTravel = 0.f;

}

void UAdvancedMovementComponent::StopDodge(bool ResetMovementMode)
{
	if (!IsDodging())
		return;

	if (ResetMovementMode)
	{
		SetMovementMode(EMovementMode::MOVE_Walking, 0);
	}

	bOrientRotationToMovement = bOldOrientRotationToMovement;
}

void UAdvancedMovementComponent::PhysDodge(float deltaTime, int32 Iterations)
{
	if (DodgeDistanceTravel >= DodgeDistance)
		StopDodge();

	DodgeDistanceTravel += DodgeSpeed * deltaTime;
	FHitResult hit;
	const auto Adjusted = DodgeDirection * deltaTime * DodgeSpeed;
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);
}

bool UAdvancedMovementComponent::IsDodging() const
{
	return (MovementMode == EMovementMode::MOVE_Custom && GetAdvancedMovementMode() == EAdvancedMovementMode::ADV_MOVE_Dodge && UpdatedComponent);
}

/** ------------------- Dodge Part End ---------------------------------- */

/** ------------------- SidesStep Part Begin ---------------------------------- */
bool UAdvancedMovementComponent::CanSidesStep() const
{
	return MovementMode == EMovementMode::MOVE_Walking;
}

void UAdvancedMovementComponent::SidesStep(const FVector& NewSidesStepDirection)
{
	if (!CanSidesStep())
		return;

	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EAdvancedMovementMode::ADV_MOVE_SidesStep);
	bOldOrientRotationToMovement = bOrientRotationToMovement;
	bOrientRotationToMovement = false;

	SidesStepDirection = (NewSidesStepDirection.IsNearlyZero()) ? CharacterOwner->GetActorForwardVector() : NewSidesStepDirection;
	SidesStepDistanceTravel = 0.f;

}

void UAdvancedMovementComponent::StopSidesStep(bool ResetMovementMode)
{
	if (!IsSidesSteping())
		return;

	if (ResetMovementMode)
	{
		SetMovementMode(EMovementMode::MOVE_Walking, 0);
	}

	bOrientRotationToMovement = bOldOrientRotationToMovement;
}

void UAdvancedMovementComponent::PhysSidesStep(float deltaTime, int32 Iterations)
{
	if (SidesStepDistanceTravel >= SidesStepDistance)
		StopSidesStep();

	SidesStepDistanceTravel += SidesStepSpeed * deltaTime;
	FHitResult hit;

	/** Try to make the player go in the SidesStep when going down to see he is not in the air */
	auto Adjusted = (SidesStepDirection - GetOwner()->GetActorUpVector()) * deltaTime * SidesStepSpeed;
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);

	if (hit.bBlockingHit)
	{
		/** If the player cant go down try to go just in the direction of the SidesStep */
		Adjusted = SidesStepDirection * deltaTime * SidesStepSpeed;
		SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);

		if (hit.bBlockingHit)
		{
			/** If the player cant go in the direction try to go in the up the direction of the SidesStep */
			Adjusted = (SidesStepDirection + GetOwner()->GetActorUpVector()) * deltaTime * SidesStepSpeed;
			SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);
		}
	}
}

bool UAdvancedMovementComponent::IsSidesSteping() const
{
	return (MovementMode == EMovementMode::MOVE_Custom && GetAdvancedMovementMode() == EAdvancedMovementMode::ADV_MOVE_SidesStep && UpdatedComponent);
}

/** ------------------- SidesStep Part End ---------------------------------- */

/** ------------------- Rush Part Begin ---------------------------------- */
bool UAdvancedMovementComponent::CanRush() const
{
	return MovementMode == EMovementMode::MOVE_Walking;
}

void UAdvancedMovementComponent::Rush(const FVector& NewRushDirection)
{
	if (!CanRush())
		return;

	SetMovementMode(EMovementMode::MOVE_Custom, (uint8)EAdvancedMovementMode::ADV_MOVE_Rush);
	bOldOrientRotationToMovement = bOrientRotationToMovement;
	bOrientRotationToMovement = false;

	RushDirection = (NewRushDirection.IsNearlyZero()) ? CharacterOwner->GetActorForwardVector() : NewRushDirection;
	RushDistanceTravel = 0.f;

}

void UAdvancedMovementComponent::StopRush(bool ResetMovementMode)
{
	if (!IsRushing())
		return;

	if (ResetMovementMode)
	{
		SetMovementMode(EMovementMode::MOVE_Walking, 0);
	}

	bOrientRotationToMovement = bOldOrientRotationToMovement;
}

void UAdvancedMovementComponent::PhysRush(float deltaTime, int32 Iterations)
{
	if (RushDistanceTravel >= RushDistance)
		StopRush();

	RushDistanceTravel += RushSpeed * deltaTime;
	FHitResult hit;
	const auto Adjusted = RushDirection * deltaTime * RushSpeed;
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentRotation(), true, hit);
}

bool UAdvancedMovementComponent::IsRushing() const
{
	return (MovementMode == EMovementMode::MOVE_Custom && GetAdvancedMovementMode() == EAdvancedMovementMode::ADV_MOVE_Rush && UpdatedComponent);
}

/** ------------------- Rush Part End ---------------------------------- */

