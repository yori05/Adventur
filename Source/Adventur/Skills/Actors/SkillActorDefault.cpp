// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActorDefault.h"

// Sets default values
ASkillActorDefault::ASkillActorDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkillActorDefault::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillActorDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

