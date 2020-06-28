// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Adventur/Interfaces/InteractibleInterface.h"
#include "TargetableCharacter.generated.h"

UCLASS()
class ADVENTUR_API ATargetableCharacter : public ACharacter //, public IInteractibleInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target, meta = (AllowPrivateAccess = "true"))
	class UTargetableComponent* TargetableComponent;

public:
	// Sets default values for this character's properties
	ATargetableCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UTargetableComponent* GetTargetableComponent() const { return TargetableComponent; }

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	//void OnBeginInteract(AActor* Caller) override;
	//
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	//void OnStopInteract(AActor* Caller) override;
	//
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	//void BeginFocus() override;
	//
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	//void StopFocus() override;

};
