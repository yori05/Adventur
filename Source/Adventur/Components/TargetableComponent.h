// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetBeginFocusSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTargetEndFocusSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ADVENTUR_API UTargetableComponent : public USceneComponent
{
	GENERATED_BODY()

protected :
	UPROPERTY(VisibleAnywhere)
	uint8 bIsLocked = 0;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsFocus = 0;

public :
	UPROPERTY(BlueprintAssignable, Category="Focus")
		FTargetBeginFocusSignature OnTargetBeginFocus;

	UPROPERTY(BlueprintAssignable, Category="Focus")
		FTargetEndFocusSignature OnTargetEndFocus;

	UPROPERTY(BlueprintAssignable, Category = "Focus")
		FTargetBeginFocusSignature OnTargetBeginLocked;

	UPROPERTY(BlueprintAssignable, Category = "Focus")
		FTargetEndFocusSignature OnTargetEndLocked;


public:	
	// Sets default values for this component's properties
	UTargetableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GetFocus();
	UFUNCTION(BlueprintCallable)
	void LoseFocus();

	UFUNCTION(BlueprintCallable)
		void GetLocked();
	UFUNCTION(BlueprintCallable)
		void LoseLocked();

};
