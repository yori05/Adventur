// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ADVENTUR_API FSlotItem
{
	GENERATED_BODY()
public:
	FSlotItem();
	~FSlotItem();

	struct FItemInfo* ItemInfo;
	uint16 Quantity;
};
