// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractibleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVENTUR_API IInteractibleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	void OnBeginInteract(AActor* Caller);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void OnStopInteract(AActor* Caller);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void BeginFocus();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void StopFocus();

};
