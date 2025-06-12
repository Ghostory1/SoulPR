// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRWeaponCollisionComponent.h"

#include "Kismet/KismetSystemLibrary.h"


USPRWeaponCollisionComponent::USPRWeaponCollisionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}



void USPRWeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void USPRWeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void USPRWeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void USPRWeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void USPRWeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMesh = MeshComponent;
}

void USPRWeaponCollisionComponent::AddIgnoredActor(AActor* Actor)
{
	IgnoredActors.Add(Actor);
}

void USPRWeaponCollisionComponent::RemoveIgnoredActor(AActor* Actor)
{
	IgnoredActors.Remove(Actor);
}

bool USPRWeaponCollisionComponent::CanHitActor(AActor* Actor) const
{
	return AlreadyHitActors.Contains(Actor) == false;
}

void USPRWeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;

	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoredActors,
		DrawDebugType,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor == nullptr)
			{
				continue;
			}

			if (CanHitActor(HitActor))
			{
				AlreadyHitActors.Add(HitActor);

				// Call OnHitActor Broadcast
				if (OnHitActor.IsBound())
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}
