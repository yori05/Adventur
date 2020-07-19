// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedPlayerController.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Adventur/Character/AdvancedCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Adventur/Components/TargetableComponent.h"
#include "Adventur/Components/LockerComponent.h"

AAdvancedPlayerController::AAdvancedPlayerController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	OffsetYaw = 0.f;
	OffsetPitch = 0.f;
	bShouldFollowTheTarget = true;
	bFollowTheTarget = false;
	bShouldAutoChangeTheLockerFocus = false;
	bCanSidesStep = false;
}

void AAdvancedPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);


	InputComponent->BindAction("Jump", IE_Pressed, this, &AAdvancedPlayerController::Jump);
	InputComponent->BindAction("StopJump", IE_Pressed, this, &AAdvancedPlayerController::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AAdvancedPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAdvancedPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APlayerController::AddYawInput);
	InputComponent->BindAxis("TurnRate", this, &AAdvancedPlayerController::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APlayerController::AddPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AAdvancedPlayerController::LookUpAtRate);
	InputComponent->BindAction("LockPress", IE_Pressed, this, &AAdvancedPlayerController::LockTargetPress);
	InputComponent->BindAction("LockHold", IE_Pressed, this, &AAdvancedPlayerController::LockTargetBegin);
	InputComponent->BindAction("LockHold", IE_Released, this, &AAdvancedPlayerController::LockTargetEnd);
	InputComponent->BindAction("LockHold", IE_Released, this, &AAdvancedPlayerController::Interact);
	InputComponent->BindKey(EKeys::I, IE_DoubleClick, this, &AAdvancedPlayerController::SidesStep);
}

void AAdvancedPlayerController::SetPawn(APawn* InPawn)
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();
	if (IsValid(MyPawn))
	{
		auto MyLocker = MyPawn->GetLockerComponent();

		if (IsValid(MyLocker))
		{
			MyLocker->OnLockerBeginFocusTarget.RemoveDynamic(this, &AAdvancedPlayerController::LockerBeginFocusTarget);
			MyLocker->OnLockerEndFocusTarget.RemoveDynamic(this, &AAdvancedPlayerController::LockerEndFocusTarget);
		}
	}

	Super::SetPawn(InPawn);

	MyPawn = GetPawn<AAdvancedCharacter>();
	if (IsValid(MyPawn))
	{
		auto MyLocker = MyPawn->GetLockerComponent();

		if (IsValid(MyLocker))
		{
			MyLocker->OnLockerBeginFocusTarget.AddUniqueDynamic(this, &AAdvancedPlayerController::LockerBeginFocusTarget);
			MyLocker->OnLockerEndFocusTarget.AddUniqueDynamic(this, &AAdvancedPlayerController::LockerEndFocusTarget);
		}
	}

}

void AAdvancedPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bFollowTheTarget)
		MakeCameraFollowTarget(DeltaTime);

	InputMotionTick(DeltaTime);
}

bool AAdvancedPlayerController::UpdateStateAxis(float DeltaTime, EDoubleTapState& DTState, float& DTTimer, float AxisValue, bool OpositeDirection)
{
	if (DTState == STATE_Ignored)
	{
		if ((!OpositeDirection && AxisValue < 0.5f) || (OpositeDirection && AxisValue > -0.5f))
		{
			DTState = STATE_Unhold;
			DTTimer = 0.f;
		}
	}
	else if (DTState == STATE_Unhold)
	{
		if ((!OpositeDirection && AxisValue >= 0.5f) || (OpositeDirection && AxisValue <= -0.5f))
		{
			DTState = STATE_Hold;
			DTTimer = 0.f;
		}
	}
	else if (DTState == STATE_Hold)
	{
		if ((!OpositeDirection && AxisValue >= 0.5f) || (OpositeDirection && AxisValue <= -0.5f))
		{
			DTTimer += DeltaTime;

			if (DTTimer > TimeHoldFirstTap)
			{
				DTState = STATE_Ignored;
				DTTimer = 0.f;
			}
		}
		else
		{
			DTState = STATE_Released;
			DTTimer = 0.f;
		}
	}
	else if (DTState == STATE_Released)
	{
		if ((!OpositeDirection && AxisValue < 0.5f) || (OpositeDirection && AxisValue > -0.5f))
		{
			DTTimer += DeltaTime;

			if (DTTimer > TimeBetwenTwoTap)
			{
				DTState = STATE_Unhold;
				DTTimer = 0.f;
			}
		}
		else
		{
			DTState = STATE_Unhold;
			DTTimer = 0.f;
			return true;
			//SidesStep();
		}
	}

	return false;
}

void AAdvancedPlayerController::InputMotionTick(float DeltaTime)
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("SidesStep State : %i Timer : %f"), DoubleTapStateForward, DoubleTapTimerForward));

	//Used to update the state of the movement axis in the 4 direction
	if (bCanSidesStep && 
		(UpdateStateAxis(DeltaTime, DoubleTapStateForward, DoubleTapTimerForward, InputForwardRight.X)
		|| UpdateStateAxis(DeltaTime, DoubleTapStateBackward, DoubleTapTimerBackward, InputForwardRight.X, true)
		|| UpdateStateAxis(DeltaTime, DoubleTapStateRight, DoubleTapTimerRight, InputForwardRight.Y)
		|| UpdateStateAxis(DeltaTime, DoubleTapStateLeft, DoubleTapTimerLeft, InputForwardRight.Y, true))
		)
	{
		SidesStep();
	}
	
	MyPawn->MoveForward(InputForwardRight.X);
	MyPawn->MoveRight(InputForwardRight.Y);
	
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, FString::Printf(TEXT("Input Value : %s"), *InputForwardRight.ToString()));
}

//Make the camera component follow the target of the lockercomponent
void AAdvancedPlayerController::MakeCameraFollowTarget(float DeltaTime)
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();
	if (!IsValid(MyPawn))
		return;

	auto MyLocker = MyPawn->GetLockerComponent();
	auto FollowCamera = MyPawn->GetFollowCamera();
	if (!IsValid(MyLocker) || !IsValid(FollowCamera))
		return;

	auto MyTarget = MyLocker->GetCurrentTarget();
	if (!IsValid(MyTarget))
	{
		bFollowTheTarget = false;
		return;
	}

	auto CamPos = FollowCamera->GetComponentLocation();
	auto TargetPos = MyTarget->GetComponentLocation();

	auto CurrentRotation = GetControlRotation();
	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(CamPos, TargetPos);
	auto InterpRot = UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 7.0f);
	FRotator NewRotator{ InterpRot.Pitch, InterpRot.Yaw,CurrentRotation.Roll };

	SetControlRotation(NewRotator);
}

void AAdvancedPlayerController::Jump()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	MyPawn->Jump();
}

void AAdvancedPlayerController::StopJumping()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	MyPawn->StopJumping();
}

void AAdvancedPlayerController::MoveForward(float Value)
{
	InputForwardRight.X = Value;

	//auto MyPawn = GetPawn<AAdvancedCharacter>();
	//
	//if (!IsValid(MyPawn))
	//	return;
	//
	//MyPawn->MoveForward(Value);
}

void AAdvancedPlayerController::MoveRight(float Value)
{
	InputForwardRight.Y = Value;

	//auto MyPawn = GetPawn<AAdvancedCharacter>();
	//
	//if (!IsValid(MyPawn))
	//	return;
	//
	//MyPawn->MoveRight(Value);
}

void AAdvancedPlayerController::SidesStep()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	// get the forward and right vector from the orientation of the controller 
	const FRotator YawRotation(0, GetControlRotation().Yaw, 0);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	MyPawn->SidesStep(ForwardVector * InputForwardRight.X + RightVector * InputForwardRight.Y);
}

void AAdvancedPlayerController::TurnAtRate(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AAdvancedPlayerController::LookUpAtRate(float Rate)
{
	if (GetWorld() != nullptr)
	{
		AddPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}


void AAdvancedPlayerController::LockTargetPress()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	if (MyPawn->HasATarget())
	{
		MyPawn->UnlockTheTarget();
		if (bShouldAutoChangeTheLockerFocus)
		MyPawn->ChangeAutoLock(false);
	}
	else
	{
		MyPawn->LockTheTarget();
		if (bShouldAutoChangeTheLockerFocus)
			MyPawn->ChangeAutoLock(true);
	}
}

void AAdvancedPlayerController::LockTargetBegin()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	MyPawn->LockTheTarget();
	if (bShouldAutoChangeTheLockerFocus)
		MyPawn->ChangeAutoLock(true);
}

void AAdvancedPlayerController::LockTargetEnd()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (!IsValid(MyPawn))
		return;

	MyPawn->UnlockTheTarget();
	if (bShouldAutoChangeTheLockerFocus)
		MyPawn->ChangeAutoLock(false);
}

void AAdvancedPlayerController::LockerBeginFocusTarget(UTargetableComponent* NewTarget)
{
	bFollowTheTarget = true;
}

void AAdvancedPlayerController::LockerEndFocusTarget(UTargetableComponent* OldTarget)
{
	bFollowTheTarget = false;
}

void AAdvancedPlayerController::Interact()
{
	auto MyPawn = GetPawn<AAdvancedCharacter>();

	if (IsValid(MyPawn))
	{
		MyPawn->Interact();
	}
}
