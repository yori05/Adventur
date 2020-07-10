// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AdvancedCharacter.generated.h"

class UTargetableComponent;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	STATE_Free UMETA(DisplayName = "Free"),
	STATE_Shield UMETA(DisplayName = "Shield"),
	STATE_Lock UMETA(DisplayName = "Lock"),
	STATE_LockShield UMETA(DisplayName = "LockShield"),
	STATE_Scope UMETA(DisplayName = "Scope"),
	STATE_Dead UMETA(DisplayName = "Dead"),
	STATE_Max UMETA(Hidden),
};

UCLASS()
class ADVENTUR_API AAdvancedCharacter : public ACharacter
{
	GENERATED_BODY()

private: 
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target, meta = (AllowPrivateAccess = "true"))
	class ULockerComponent* LockerComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Target, meta = (AllowPrivateAccess = "true"))
	class UPrimitiveComponent* DetectionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	mutable class UAdvancedMovementComponent* AdvancedMovementComponent;

protected :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::STATE_Free;
public:
	// Sets default values for this character's properties
	AAdvancedCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void SidesStep(const FVector & MovementDirection);

	UFUNCTION(BlueprintCallable)
		void LockTheTarget();
	UFUNCTION(BlueprintCallable)
		void UnlockTheTarget();

	UFUNCTION(BlueprintCallable)
		bool HasATarget();
	UFUNCTION(BlueprintCallable)
	UTargetableComponent* GetCurrentTarget();

	UFUNCTION(BlueprintCallable)
		void ChangeAutoLock(bool NewAutoLock);

	UFUNCTION(BlueprintCallable)
	void ChangeState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	ECharacterState GetState();

	UFUNCTION(BlueprintCallable)
	void Interact();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class ULockerComponent* GetLockerComponent() const { return LockerComponent; }

	FORCEINLINE class UPrimitiveComponent* GetDetectionComponent() const { return DetectionComponent; }

	FORCEINLINE class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	FORCEINLINE class UAdvancedMovementComponent* GetAdvancedMovementCompnent() const { return AdvancedMovementComponent; }
};
