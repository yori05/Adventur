// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AdvancedPlayerController.generated.h"

UENUM(BlueprintType)
enum EDoubleTapState
{
	/** Unhold default state take when the double tap axis is keep unhold */
	STATE_Unhold UMETA(DisplayName = "Unhold"),
	/** Hold state take when the double tap axis is hold for the first time and the timer of axis is less than the max timer, if not will go in Ignored State */
	STATE_Hold UMETA(DisplayName = "Hold"),
	/** Released state take when the double tap axis is released after have been hold for the first time and the timer of axis is less than the max timer, if not will go in Unhold State */
	STATE_Released UMETA(DisplayName = "Released"),
	/** Ignored state take when the double tap axis is hold for too long */
	STATE_Ignored UMETA(DisplayName = "Ignored"),
};

/**
 * 
 */
UCLASS()
class ADVENTUR_API AAdvancedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float OffsetYaw;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float OffsetPitch;

protected :
	//The value recorded by the input Forward And Right
	FVector2D InputForwardRight;

	EDoubleTapState DoubleTapStateForward;
	EDoubleTapState DoubleTapStateRight;
	EDoubleTapState DoubleTapStateBackward;
	EDoubleTapState DoubleTapStateLeft;

	float DoubleTapTimerForward = 0.f;
	float DoubleTapTimerRight = 0.f;
	float DoubleTapTimerBackward = 0.f;
	float DoubleTapTimerLeft = 0.f;

	//Time max to hold the first tap to consider it at the initiation of a double tap
	UPROPERTY(EditAnywhere, Category = "Input|Movement|DoubleTap")
		float TimeHoldFirstTap = 0.25f;
	//Time max betwen the two tap to consider it at a double tap
	UPROPERTY(EditAnywhere, Category = "Input|Movement|DoubleTap")
		float TimeBetwenTwoTap = 0.25f;

	// Locker component of the character part
	UPROPERTY(EditAnywhere, Category = "AdvancedMovement|SidesStep")
		uint8 bCanSidesStep : 1; //Should the camera follow the target of the locker component
	UPROPERTY(EditAnywhere, Category = "Locker|Camera")
		uint8 bShouldFollowTheTarget : 1; //Should the camera follow the target of the locker component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Locker|Camera")
		uint8 bFollowTheTarget : 1; //To launch the follow on the update
	UPROPERTY(EditAnywhere, Category = "Locker|Camera")
		uint8 bShouldAutoChangeTheLockerFocus : 1; //When we active / desactive the lock of the target with the locker component. Should the controller active / desactive the auto change of the component


public :
	AAdvancedPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected :
	virtual void SetPawn(APawn* InPawn) override;

	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void MakeCameraFollowTarget(float DeltaTime);
	virtual void InputMotionTick(float DeltaTime);

	/**
	 * Used to update the state of a axis using a EDoubleTapState and a Timer. Can choice if we want a value in the oposite of the direction
	 * Return true if the axis is hold enought time, and enought time pass before he is hold again
	 * Using the value of TimeHoldFirstTap and TimeBetwenTwoTap
	 */ 
	bool UpdateStateAxis(float DeltaTime, EDoubleTapState& DTState, float& DTTimer, float AxisValue, bool OpositeDirection = false);

	void Jump();
	void StopJumping();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void SidesStep();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void LockTargetPress();
	void LockTargetBegin();
	void LockTargetEnd();

	void Interact();

	UFUNCTION(BlueprintCallable)
	virtual void LockerBeginFocusTarget(class UTargetableComponent * NewTarget);
	UFUNCTION(BlueprintCallable)
	virtual void LockerEndFocusTarget(class UTargetableComponent * OldTarget);

};
