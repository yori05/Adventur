// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Adventur/Items/SlotItem.h"
#include "InventoryComponent.generated.h"

//struct FSlotItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class ADVENTUR_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FText Name = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
	uint16 NumberOfSlot = 0;

	UPROPERTY(EditAnywhere)
		TArray<FSlotItem> Inventory;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
