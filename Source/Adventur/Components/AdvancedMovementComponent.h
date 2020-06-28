// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedMovementComponent.generated.h"

UENUM(BlueprintType, Category = "CustomMovement")
enum class EAdvancedMovementMode : uint8
{
	ADV_MOVE_None UMETA(DisplayName = "None"),
	ADV_MOVE_Dodge UMETA(DisplayName = "Dodge"), // Will make the character dodge with a jump in the direction
	ADV_MOVE_SidesStep UMETA(DisplayName = "SidesStep"), // Will make the character dodge in a direction with a quick rush
	ADV_MOVE_Rush UMETA(DisplayName = "Rush"), // Will make the character rush in a direction
	ADV_MOVE_Max UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class ADVENTUR_API UAdvancedMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private : 
	//------------ Advanced Movement Mode Private Parametter -----------

	// The following value will be used when we launch a SidesStep / Dodge / Rush to save the direction of the movement

	// For the Dodge movement mode
	FVector DodgeDirection;

	// For the SidesStep movement mode
	FVector SidesStepDirection;

	// For the Rush movement mode
	FVector RushDirection;

	// The following value will be used to keep the progression of the folling movement mode : SidesStep / Dodge / Rush

	// For the Dodge movement mode
	float DodgeDistanceTravel;
	
	// For the SidesStep movement mode
	float SidesStepDistanceTravel;

	// For the Rush movement mode
	float RushDistanceTravel;
	
	// Use to save the previous orien to movement in the mode that can change it
	bool bOldOrientRotationToMovement;


public :
	/** ----- Dodge Public Parametter ----------------------*/
	/** The Dodge's speed  */
	UPROPERTY(Category = "Advanced Movement (Dodge Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float DodgeSpeed = 500.f;
	/** The Dodge's Distance before it stopped  */
	UPROPERTY(Category = "Advanced Movement (Dodge Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float DodgeDistance = 500.f;
	/** ----- Dodge Public Parametter End ----------------------*/

	/** ----- SidesStep Public Parametter ----------------------*/
	/** The SidesStep's speed  */
	UPROPERTY(Category = "Advanced Movement (SidesStep Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float SidesStepSpeed = 1000.f;
	/** The SidesStep's Distance before it stopped  */
	UPROPERTY(Category = "Advanced Movement (SidesStep Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float SidesStepDistance = 250.f;
	/** ----- SidesStep Public Parametter End ----------------------*/

	/** ----- Rush Public Parametter ----------------------*/
	/** The Rush's speed  */
	UPROPERTY(Category = "Advanced Movement (Rush Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float RushSpeed = 750.0f;
	/** The Rush's Distance before it stopped  */
	UPROPERTY(Category = "Advanced Movement (Rush Settings)", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
		float RushDistance = 1000.f;
	/** ----- Rush Public Parametter End ----------------------*/


public :
	
	/** Return value of the CustomMovement in a uint8 value */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual uint8 GetCustomMovementMode() const;

	/** Return value of the CustomMovement in a EAdvancedMovementMode value */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual EAdvancedMovementMode GetAdvancedMovementMode() const;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	//---------------- Dodge Part Begin --------------------- //
	
	/**
	 *  Use to know if the movement mode can be set to Dodge
	 *  Return false if :
	 *		The character is not in the movement mode walking 
	 *  
	 *  You can overide this function to launch it on other event
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool CanDodge() const;

	/**
	 * Will launch the movement mode to Dodge
	 *@param NewDirectionDodge is the direction in which the movement will be apply.
	 *	note that if the vector is set to null it will used on the forward to the character
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void Dodge(const FVector & NewDodgeDirection);

	/**
	 * Use to stop the dodge movement mode 
	 *@param ResetMovementMode if set as true will set the movement mode to Walking
	**/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void StopDodge(bool ResetMovementMode = true);

	//The physic of the dodge movement mode
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void PhysDodge(float deltaTime, int32 Iterations);

	//Will return true if the MovementMode is in custom + CustomMovementMode is set as ADV_MOVE_Dodge and UpdatedMovement is set as true
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool IsDodging() const;

	//---------------- Dodge Part End --------------------- //

	//---------------- SidesStep Part Begin --------------------- //

	/**
	 *  Use to know if the movement mode can be set to SidesStep
	 *  Return false if :
	 *		The character is not in the movement mode walking
	 *
	 *  You can overide this function to launch it on other event
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool CanSidesStep() const;

	/**
	 * Will launch the movement mode to SidesStep
	 *@param NewDirectionSidesStep is the direction in which the movement will be apply.
	 *	note that if the vector is set to null it will used on the forward to the character
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void SidesStep(const FVector& NewSidesStepDirection);

	/**
	 * Use to stop the SidesStep movement mode
	 *@param ResetMovementMode if set as true will set the movement mode to Walking
	**/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void StopSidesStep(bool ResetMovementMode = true);

	//The physic of the SidesStep movement mode
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void PhysSidesStep(float deltaTime, int32 Iterations);

	//Will return true if the MovementMode is in custom + CustomMovementMode is set as ADV_MOVE_SidesStep and UpdatedMovement is set as true
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool IsSidesSteping() const;

	//---------------- SidesStep Part End --------------------- //

	//---------------- Rush Part Begin --------------------- //

	/**
	 *  Use to know if the movement mode can be set to Rush
	 *  Return false if :
	 *		The character is not in the movement mode walking
	 *
	 *  You can overide this function to launch it on other event
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool CanRush() const;

	/**
	 * Will launch the movement mode to Rush
	 *@param NewDirectionRush is the direction in which the movement will be apply.
	 *	note that if the vector is set to null it will used on the forward to the character
	*/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void Rush(const FVector& NewRushDirection);

	/**
	 * Use to stop the Rush movement mode
	 *@param ResetMovementMode if set as true will set the movement mode to Walking
	**/
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void StopRush(bool ResetMovementMode = true);

	//The physic of the Rush movement mode
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual void PhysRush(float deltaTime, int32 Iterations);

	//Will return true if the MovementMode is in custom + CustomMovementMode is set as ADV_MOVE_Rush and UpdatedMovement is set as true
	UFUNCTION(BlueprintCallable, Category = "Pawn|Components|CharacterMovement|AdvancedMovement")
		virtual bool IsRushing() const;

	//---------------- Rush Part End --------------------- //

};
