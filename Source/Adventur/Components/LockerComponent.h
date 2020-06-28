// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockerComponent.generated.h"

class UPrimitiveComponent;
class UTargetableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockerChangeAutoFocusSignature, uint8, NewAutoFocus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockerLoseTargetSignature, UTargetableComponent*, OldTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockerGetTargetSignature, UTargetableComponent*, NewTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockerBeginFocusTargetSignature, UTargetableComponent*, NewFocusTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLockerEndFocusTargetSignature, UTargetableComponent*, OldFocusTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLockerChangeFocusTargetSignature, UTargetableComponent*, NewFocusTarget, UTargetableComponent*, OldFocusTarget);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ADVENTUR_API ULockerComponent : public UActorComponent
{
	GENERATED_BODY()

protected :
	UPROPERTY(EditAnywhere)
	UPrimitiveComponent *DetectionComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	TArray<UTargetableComponent*> Targets;

	UPROPERTY(VisibleAnywhere)
	UTargetableComponent* CurrentTarget = nullptr;

	UPROPERTY(VisibleAnywhere)
		UTargetableComponent* GreaterTarget = nullptr;

	UPROPERTY(EditAnywhere)
	uint8 bAutoLock = false;

public :
	UPROPERTY(BlueprintAssignable)
		FLockerLoseTargetSignature OnLockerLoseTarget;
	UPROPERTY(BlueprintAssignable)
		FLockerGetTargetSignature OnLockerGetTarget;
	UPROPERTY(BlueprintAssignable)
		FLockerBeginFocusTargetSignature OnLockerBeginFocusTarget;
	UPROPERTY(BlueprintAssignable)
		FLockerChangeFocusTargetSignature OnLockerChangeFocusTarget;
	UPROPERTY(BlueprintAssignable)
		FLockerEndFocusTargetSignature OnLockerEndFocusTarget;
	UPROPERTY(BlueprintAssignable)
		FLockerChangeAutoFocusSignature OnChangeAutoFocus;

public:	
	// Sets default values for this component's properties
	ULockerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void DetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SortTargets();
	void FindNewTargets();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetDetectionComponent(UPrimitiveComponent* NewDetectionComponent);
	
	UFUNCTION(BlueprintCallable)
	void LockGreatestTarget();

	UFUNCTION(BlueprintCallable)
	void UnlockTarget();

	UFUNCTION(BlueprintCallable)
	UTargetableComponent* GetCurrentTarget() { return CurrentTarget; };

	UFUNCTION(BlueprintCallable)
	UTargetableComponent* GetGreatestTarget() { return GreaterTarget; };

	UFUNCTION(BlueprintCallable)
	bool HasATarget();

	UFUNCTION(BlueprintCallable)
		void ChangeAutoLock(uint8 NewAutoFocus);

	UFUNCTION(BlueprintCallable)
		uint8 GetAutoLock() { return bAutoLock; };
};
