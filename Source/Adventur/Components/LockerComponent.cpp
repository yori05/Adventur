// Fill out your copyright notice in the Description page of Project Settings.


#include "LockerComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Adventur/Components/TargetableComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
ULockerComponent::ULockerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULockerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bAutoLock)
		FindNewTargets();
	else if (!IsValid(CurrentTarget))
	SortTargets();
	// ...
}

void ULockerComponent::FindNewTargets()
{
	SortTargets();
	if (IsValid(GreaterTarget))
	{
		LockGreatestTarget();
	}
}

void ULockerComponent::SortTargets()
{
	if (Targets.Num() == 0)
		return;

	auto World = GetWorld();

	if (!IsValid(World))
		return;

	auto MainPC = World->GetFirstPlayerController();
	auto ActorOwner = GetOwner();

	if (!IsValid(MainPC) || !IsValid(ActorOwner))
		return;

	FVector2D closestPos;
	UTargetableComponent* BetterTarget = nullptr;
	FVector2D tempPos;
	FVector2D ownerPos; 
	if (MainPC->ProjectWorldLocationToScreen(ActorOwner->GetActorLocation(), tempPos))
		ownerPos = tempPos;
	else
	{
		int32 sizeX;
		int32 sizeY;
		MainPC->GetViewportSize(sizeX, sizeY);
		ownerPos.X = sizeX;
		ownerPos.Y = sizeY;
	}
	for (auto &i : Targets)
	{
		if (MainPC->ProjectWorldLocationToScreen(i->GetComponentLocation(), tempPos))
		{
			if (BetterTarget == nullptr || closestPos.SizeSquared() > (tempPos - ownerPos).SizeSquared())
			{
				closestPos = tempPos - ownerPos;
				BetterTarget = i;
			}
		}
	}

	if (BetterTarget != nullptr && GreaterTarget != BetterTarget)
	{
		if (IsValid(GreaterTarget))
		{
			GreaterTarget->LoseFocus();
		}

		GreaterTarget = BetterTarget;
		GreaterTarget->GetFocus();
		int32 sizeX;
		int32 sizeY;
		MainPC->GetViewportSize(sizeX, sizeY);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Blue, FString::Printf(TEXT("ScreenSize : %d | %d"), sizeX, sizeY),true);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(12, 1.f, FColor::Red, FString::Printf(TEXT("Pos On Screen : %f | %f"), closestPos.X, closestPos.Y),true);
	}
	else if (BetterTarget == nullptr)
	{
		if (IsValid(GreaterTarget))
			GreaterTarget->LoseFocus();
		GreaterTarget = nullptr;
	}
}

void ULockerComponent::DetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentTags.Contains(FName(TEXT("Targetable"))))
	{
		auto NewComp = OtherActor->GetComponentByClass(UTargetableComponent::StaticClass());
		auto NewTarget = Cast<UTargetableComponent>(NewComp);

		if (IsValid(NewTarget))
		{
			Targets.Add(NewTarget);
			OnLockerGetTarget.Broadcast(NewTarget);

			if (bAutoLock)
			{
				FindNewTargets();
			}

			OnDetectionBeginOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
		}
	}
}

void ULockerComponent::DetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentTags.Contains(FName(TEXT("Targetable"))))
	{
		auto OldComp = OtherActor->GetComponentByClass(UTargetableComponent::StaticClass());
		auto OldTarget = Cast<UTargetableComponent>(OldComp);
		if (IsValid(OldTarget) && Targets.Contains(OldTarget))
		{
			Targets.Remove(OldTarget);
			OnLockerLoseTarget.Broadcast(OldTarget);

			if (OldTarget == GreaterTarget)
			{
				GreaterTarget->LoseFocus();
				GreaterTarget = nullptr;
			}
			if (OldTarget == CurrentTarget)
			{
				CurrentTarget->LoseLocked();
				CurrentTarget = nullptr;
				
				if (bAutoLock)
				{
					FindNewTargets();
				}
				else
				{
					OnLockerEndFocusTarget.Broadcast(OldTarget);
				}
			}

			OnDetectionEndOverlap.Broadcast(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
		}
	}
}

void ULockerComponent::SetDetectionComponent(UPrimitiveComponent* NewDetectionComponent)
{
	if (IsValid(DetectionComponent))
	{
		DetectionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ULockerComponent::DetectionBeginOverlap);
		DetectionComponent->OnComponentEndOverlap.RemoveDynamic(this, &ULockerComponent::DetectionEndOverlap);
	}

	DetectionComponent = NewDetectionComponent;

	if (IsValid(DetectionComponent))
	{
		DetectionComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ULockerComponent::DetectionBeginOverlap);
		DetectionComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &ULockerComponent::DetectionEndOverlap);
	}
}

void ULockerComponent::LockGreatestTarget()
{
	if (IsValid(GreaterTarget))
	{
		GreaterTarget->LoseFocus();
		if (IsValid(CurrentTarget))
		{
			CurrentTarget->LoseLocked();
			OnLockerChangeFocusTarget.Broadcast(GreaterTarget, CurrentTarget);
		}
		else
		{
			OnLockerBeginFocusTarget.Broadcast(GreaterTarget);
		}
		CurrentTarget = GreaterTarget;
		CurrentTarget->GetLocked();
	}
}

void ULockerComponent::UnlockTarget()
{
	if (IsValid(CurrentTarget))
	{
		CurrentTarget->LoseLocked();
		GreaterTarget = CurrentTarget;
		GreaterTarget->GetFocus();
		OnLockerEndFocusTarget.Broadcast(CurrentTarget);
		CurrentTarget = nullptr;
	}
}

bool ULockerComponent::HasATarget() 
{
	return IsValid(CurrentTarget); 
}

void ULockerComponent::ChangeAutoLock(uint8 NewAutoFocus) 
{
	bAutoLock = NewAutoFocus; 
	OnChangeAutoFocus.Broadcast(NewAutoFocus);
};
