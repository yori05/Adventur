// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetableCharacter.h"
#include "Adventur/Components/TargetableComponent.h"

// Sets default values
ATargetableCharacter::ATargetableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TargetableComponent = CreateDefaultSubobject<UTargetableComponent>(TEXT("TargetableComponent"));
	TargetableComponent->SetupAttachment(GetMesh());
	
}

// Called when the game starts or when spawned
void ATargetableCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATargetableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATargetableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//void ATargetableCharacter::OnBeginInteract(AActor* Caller)
//{
//
//}
//
//void ATargetableCharacter::OnStopInteract(AActor* Caller)
//{
//
//}
//
//void ATargetableCharacter::BeginFocus()
//{
//
//}
//
//void ATargetableCharacter::StopFocus()
//{
//
//}
