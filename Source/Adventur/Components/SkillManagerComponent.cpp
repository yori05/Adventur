// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillManagerComponent.h"
#include "Adventur/Skills/Bases/SkillBase.h"
#include "Adventur/Character/AdvancedCharacter.h"

// Sets default values for this component's properties
USkillManagerComponent::USkillManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AAdvancedCharacter>(GetOwner());

	for (auto it : Skills)
	{
		//it->Initialize(this);
	}
}


// Called every frame
void USkillManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto it : Skills)
	{
		//it->TickSkill(DeltaTime);
	}
}

AAdvancedCharacter* USkillManagerComponent::GetCharacterOwner() const
{
	return OwnerCharacter; 
}

bool USkillManagerComponent::SkillRequested(const int32 SkillIndex)
{
	return false;
}

bool USkillManagerComponent::IsASkillExecuting() const
{
	return false;
}

bool USkillManagerComponent::IsASkillAvaible(const int32 SkillIndex) const
{
	return false;
}
