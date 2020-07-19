// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Adventur/Interfaces/TeleportableInterface.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRoot->SetupAttachment(GetRootComponent());

	ReverseSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ReverseSceneRoot"));
	ReverseSceneRoot->SetupAttachment(GetRootComponent());
	ReverseSceneRoot->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
	PortalMesh->SetupAttachment(DefaultSceneRoot);
	PortalMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	PortalMesh->OnComponentBeginOverlap.AddUniqueDynamic(this, &APortal::OnPortalBeginOverlap);
	PortalMesh->OnComponentEndOverlap.AddUniqueDynamic(this, &APortal::OnPortalEndOverlap);

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		//ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Characters;
		//ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Characters;
		ArrowComponent->SetupAttachment(DefaultSceneRoot);
		ArrowComponent->bIsScreenSizeScaled = true;
		//ArrowComponent->SetRelativeRotation(FRotator(00.0f, 90.f, 0.f));
	}
	ReverseArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("ReverseArrow"));
	if (ReverseArrowComponent)
	{
		ReverseArrowComponent->ArrowColor = FColor(255, 200, 150);
		ReverseArrowComponent->bTreatAsASprite = true;
		//ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Characters;
		//ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Characters;
		ReverseArrowComponent->SetupAttachment(ReverseSceneRoot);
		ReverseArrowComponent->bIsScreenSizeScaled = true;
		//ArrowComponent->SetRelativeRotation(FRotator(00.0f, 90.f, 0.f));
	}

#endif // WITH_EDITORONLY_DATA

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	TeleportableArray.Reserve(100);
	//TeleportableArray.Empty();
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* Temp = nullptr;
	float Distance = 0.0f;
	int Count = 0;

	while (Count < TeleportableArray.Num())
	{
		Temp = Cast<AActor>(TeleportableArray[Count]);
	
		if (Temp != nullptr)
		{
			Distance = FVector::DotProduct(Temp->GetActorLocation() - GetActorLocation(), GetActorForwardVector());
			if (Distance * Distance < PortalSensitivity * PortalSensitivity)
			{
				TeleportableArray[Count]->Execute_SwapLink(Temp);
			}
			else
			{
				Count++;
			}
		}
	}
}

void APortal::OnPortalBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Target = Cast<ITeleportableInterface>(OtherActor);
	if (Target != nullptr)
	{
		if (Arrival != nullptr && Target->Execute_LaunchALink(OtherActor, this, Arrival))
		{
			if (!TeleportableArray.Contains(Target))
			TeleportableArray.Add(Target);
		}
	}
}
void APortal::OnPortalEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto Target = Cast<ITeleportableInterface>(OtherActor);
	if (Target != nullptr)
	{
		if (TeleportableArray.Contains(Target))
			TeleportableArray.Remove(Target);

		if (Target->Execute_IsMainPortal(OtherActor, this))
		{
			Target->Execute_BreakLink(OtherActor);	
		}
	}
}
