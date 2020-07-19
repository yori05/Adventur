// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class USceneComponent;

UCLASS()
class ADVENTUR_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

private:
#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
		UArrowComponent* ArrowComponent;
	UPROPERTY()
		UArrowComponent* ReverseArrowComponent;

#endif

	TArray<class ITeleportableInterface*> TeleportableArray;

protected: 
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PortalMesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	USceneComponent* ReverseSceneRoot;

public :
	UPROPERTY(EditAnywhere)
		APortal* Arrival = nullptr;

	/** Distance where the portal will teleport the ITeleportableInterface on the other portal */
	UPROPERTY(EditAnywhere)
	float PortalSensitivity = 10.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void OnPortalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	virtual void OnPortalEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	FORCEINLINE USceneComponent* GetReverseSceneRoot() { return ReverseSceneRoot; }
};
