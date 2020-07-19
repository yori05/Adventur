// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeleportableInterface.generated.h"

class APortal;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UTeleportableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class ADVENTUR_API ITeleportableInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
	void LaunchTeleport(AActor* ToTeleport, FVector RelativePosition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
	void ReleaseTeleport();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
	bool IsTeleportationRunning();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
		bool LaunchALink(APortal* NewMainPortal, APortal* NewSecondaryPortal);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
		bool IsLinked(APortal* Portal);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
		bool IsMainPortal(APortal* Portal);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
		void SwapLink();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Teleportation")
		void BreakLink();

	virtual bool LaunchALink_Implementation(APortal* NewMainPortal, APortal* NewSecondaryPortal);

	virtual bool IsLinked_Implementation(APortal* Portal);

	virtual bool IsMainPortal_Implementation(APortal* Portal);

	virtual void SwapLink_Implementation();

	virtual void BreakLink_Implementation();

protected:
	bool bIsRunning = false;

	APortal* MainPortal = nullptr;
	APortal* SecondaryPortal = nullptr;
};