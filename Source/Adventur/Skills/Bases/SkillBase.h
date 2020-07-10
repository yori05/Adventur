// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSkillDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillFloatDelegate, float, DeltaSecond);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkillBoolDelegate, bool, SuccessFullRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FSkillActorBeginOverlapSignature, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSkillActorEndOverlapSignature, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex);

class USkillManagerComponent;
class ASkillActor;
class AAdvancedCharacter;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInLineNew)
class ADVENTUR_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillDelegate OnInitialize;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillFloatDelegate OnTick;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillDelegate OnExecuted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillDelegate OnBeforeSpawnActor;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillDelegate OnHalfwaySpawnActor;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillDelegate OnAfterSpawnActor;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillDelegate OnEndExecution;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillBoolDelegate OnRequestExecuted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillActorBeginOverlapSignature OnSkillActorBeginOverlap;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
		FSkillActorEndOverlapSignature OnSkillActorEndOverlap;


//rotected:
//	UPROPERTY(BlueprintReadOnly, Category = "MainData")
//		USkillManagerComponent* pOwner = nullptr;
//
//	UPROPERTY(BlueprintReadOnly, Category = "MainData")
//		AAdvancedCharacter* pTarget = nullptr;
//
//	UPROPERTY(BlueprintReadOnly, Category = "MainData")
//		ASkillActor* pActor = nullptr;
//
//	UPROPERTY(EditAnywhere, Category = "Actor")
//		TSubclassOf<class ASkillActor> TypeOfSkillActor = nullptr;
//
//	UPROPERTY(EditAnywhere, Category = "MainData")
//		float Cooldown = 0.0f;
//
//	UPROPERTY(BlueprintReadOnly, Category = "MainData")
//		float CooldownTimer = 0.0f;
//
//	UPROPERTY(EditAnywhere, Category = "Damage")
//		float DamageValue = 0.0f;
//
//	UPROPERTY(EditAnywhere, Category = "Damage")
//		ETypeElement DamageElement = ETypeElement::DamageResult_None;
//
//	UPROPERTY(BlueprintReadOnly, Category = "MainData")
//		uint8 bIsExecuted = 0;
//
//	UPROPERTY(EditAnywhere, Category = "MainData")
//		uint8 bKillActorEndExecute = 1; //if set to false you will have to destroy the SkillActorManualy later
//
//	UPROPERTY(EditAnywhere, Category = "MainData")
//		uint8 bLinkActorToTarget = 1;
//
//	UPROPERTY(EditAnywhere, Category = "MainData")
//		uint8 bIsLocker = 0; //Set if this skill lock other skill when he is executed;
//
//	UPROPERTY(EditAnywhere, Category = "MainData")
//		uint8 bIsForced = 0; //Set if this skill can be launch even if other skill lock the launch;
//
//rotected:
//	UFUNCTION(BlueprintCallable)
//		virtual void BeforeSpawnActor();
//
//	UFUNCTION(BlueprintCallable)
//		virtual void HalfWaySpawnActor();
//
//	UFUNCTION(BlueprintCallable)
//		virtual void AfterSpawnActor();
//
//ublic:
//	UFUNCTION(BlueprintCallable)
//		virtual void Initialize(USkillManagerComponent* _Owner);
//
//	UFUNCTION(BlueprintCallable)
//		virtual void TickSkill(float DeltaSecond);
//
//	UFUNCTION(BlueprintCallable)
//		virtual void Execute();
//
//	UFUNCTION(BlueprintCallable)
//		virtual void EndExecute();
//
//	UFUNCTION(BlueprintCallable)
//		virtual uint8 RequestExecute();
//
//	UFUNCTION(BlueprintCallable)
//		uint8 IsExecuted() { return bIsExecuted; }
//
//	UFUNCTION(BlueprintCallable)
//		void LaunchRecuperation() { CooldownTimer = Cooldown; }
//
//	UFUNCTION(BlueprintCallable)
//		virtual uint8 CanBeExecuted() { return pOwner && !(CooldownTimer > 0 || bIsExecuted); }
//
//	UFUNCTION(BlueprintCallable)
//		uint8 IsLocked() { return (bIsLocker) ? bIsExecuted : bIsLocker; }
//
//	UFUNCTION(BlueprintCallable)
//		uint8 IsForced() { return bIsForced; }
//
//	//What will be done when the Collider linked to this func will Begin Overlap
//	UFUNCTION(BlueprintCallable)
//		virtual void FDetectionColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
//
//	//What will be done when the Collider linked to this func will End Overlap
//	UFUNCTION(BlueprintCallable)
//		virtual void DetectionColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
//
//	//Used to launch FDetectionColliderBeginOverlap easly from a funcPointer
//	static void execFDetectiocColliderBeginOverlap(USkillBase* SkillLauncher, UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//	{
//		if (SkillLauncher)
//			SkillLauncher->FDetectionColliderBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
//	}
//
//	//Used to launch DetectionColliderEndOverlap easly from a funcPointer
//	static void FDetectiocColliderEndOverlap_Static(USkillBase* SkillLauncher, UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//	{
//		if (SkillLauncher)
//			SkillLauncher->DetectionColliderEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
//	}
//
//ublic:
//	UFUNCTION(BlueprintCallable)
//		ASkillActor* GetSkillActor() { return pActor; }
//
//	UFUNCTION(BlueprintCallable)
//		AIuniusCharacter* GetTarget() { return pTarget; }
//
//	UFUNCTION(BlueprintCallable)
//		void SetTarget(AIuniusCharacter* Target) { pTarget = Target; }
//
//	UFUNCTION(BlueprintCallable)
//		USkillManagerComponent* GetSkillManager() { return pOwner; }
//
//	UFUNCTION(BlueprintCallable)
//		void SetSkillManager(USkillManagerComponent* _Owner) { pOwner = _Owner; }
//
//
};
