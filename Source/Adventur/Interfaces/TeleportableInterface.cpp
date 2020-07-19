// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportableInterface.h"
#include "Adventur/Portals/Portal.h"

// Add default functionality here for any IInteractibleInterface functions that are not pure virtual.

//void LaunchALink(APortal* NewPortalEnter, APortal* NewPortalExit);

bool ITeleportableInterface::LaunchALink_Implementation(APortal* NewMainPortal, APortal* NewSecondaryPortal)
{
	if (IsValid(NewMainPortal) && IsValid(NewSecondaryPortal))
	{
		MainPortal = NewMainPortal;
		SecondaryPortal = NewSecondaryPortal;
		
		return true;
	}

	return false;
}

bool ITeleportableInterface::IsLinked_Implementation(APortal* Portal)
{
	return (IsValid(Portal) && (Portal == MainPortal || Portal == SecondaryPortal));
}

bool ITeleportableInterface::IsMainPortal_Implementation(APortal* Portal)
{
	return IsValid(Portal) && Portal == MainPortal;
}

void ITeleportableInterface::SwapLink_Implementation()
{
	AActor* MyActor = Cast<AActor>(this);

	if (IsValid(MyActor) && IsValid(MainPortal) && IsValid(SecondaryPortal))
	{
		FVector MainRelativePosition = MyActor->GetActorLocation() - MainPortal->GetActorLocation();				/** the relative position of the actor on the portal in world coordinate */
		FVector ScalePosition(																						/** the relative position of the actor on the portal using a scale on the direction axis of the portal */
			FVector::DotProduct(MainRelativePosition, MainPortal->GetReverseSceneRoot()->GetRightVector()),							/** X Parametter using Right */
			SecondaryPortal->PortalSensitivity * 1.5f * (FVector::DotProduct(MainRelativePosition, MainPortal->GetActorForwardVector()) < 0.f ? -1 : 1),	/** Y Parametter we don't use the Forward  vector to have distance farther than the teleportation sensibility*/
			FVector::DotProduct(MainRelativePosition, MainPortal->GetActorUpVector())								/** Z Parametter using Up*/
							);							

		auto FinalPosition = SecondaryPortal->GetActorLocation();
		FinalPosition += ScalePosition.X * SecondaryPortal->GetActorRightVector();
		FinalPosition += ScalePosition.Y * SecondaryPortal->GetActorForwardVector();
		FinalPosition += ScalePosition.Z * SecondaryPortal->GetActorUpVector();

		auto MainRelativeRotation = MainPortal->GetReverseSceneRoot()->GetComponentTransform().InverseTransformRotation(MyActor->GetActorQuat());
		auto FinalRotation = SecondaryPortal->GetActorTransform().TransformRotation(MainRelativeRotation);
		MyActor->SetActorLocationAndRotation(FinalPosition,FinalRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void ITeleportableInterface::BreakLink_Implementation()
{
	MainPortal = nullptr;
	SecondaryPortal = nullptr;
}
