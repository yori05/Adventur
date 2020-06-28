// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "ItemInfo.generated.h"

class UTexture2D;
class AItemActor;

/**
 * 
 */
USTRUCT(BlueprintType)
struct ADVENTUR_API FItemInfo
{
	GENERATED_BODY()

public:
	FItemInfo();
	~FItemInfo();

	UPROPERTY(EditAnywhere)
		FText Name = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
		FText Description = FText::GetEmpty();

	UPROPERTY(EditAnywhere)
		UTexture2D* Thumbernail = nullptr;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AItemActor> ClassItem = nullptr;

	UPROPERTY(EditAnywhere)
		AItemActor* InstanceItem = nullptr;

	UPROPERTY(EditAnywhere)
		int32 StackMax = 1;

	UPROPERTY(EditAnywhere)
	float Durability = 0.f;

	UPROPERTY(EditAnywhere)
		float Weight = 0.f;

	UPROPERTY(EditAnywhere)
	bool bIsConsumable = false;

	UPROPERTY(EditAnywhere)
		bool bIsStackable = false;


};
