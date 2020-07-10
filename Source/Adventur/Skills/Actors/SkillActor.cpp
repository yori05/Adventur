// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"

// Sets default values
ASkillActor::ASkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

