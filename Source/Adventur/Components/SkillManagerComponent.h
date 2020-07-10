// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillManagerComponent.generated.h"

class AAdvancedCharacter;
class USkillBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillMngrIntDelegate, const int32, SkillIndex);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVENTUR_API USkillManagerComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	/**
	 * List of SkillBase that can be used by component
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TArray<USkillBase*> Skills;

	/**
	 * Pointer of owner with a AAdvancedCharacter 
	 * Maybe will be remplaced by the combat manager component later 
	 */
	AAdvancedCharacter* OwnerCharacter = nullptr;

public:	
	// Sets default values for this component's properties
	USkillManagerComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Will return the character owner casted as a AAdvanchedCharacter */
	UFUNCTION(BlueprintCallable)
	AAdvancedCharacter* GetCharacterOwner() const;

	/** Request to launch a skill, will be launched if no other skill block the launch of this one and if he is not already launched or in colddown */
	UFUNCTION(BlueprintCallable)
	bool SkillRequested(const int32 SkillIndex);

	/** Will return if a skill is executing (usefull ?)*/
	UFUNCTION(BlueprintCallable)
	bool IsASkillExecuting() const;

	/** Will return if a skill is avaible */
	UFUNCTION(BlueprintCallable)
	bool IsASkillAvaible(const int32 SkillIndex) const;


};
