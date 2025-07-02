// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRRotationComponent.h"

#include "Kismet/KismetMathLibrary.h"
// Sets default values for this component's properties
USPRRotationComponent::USPRRotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USPRRotationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USPRRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetActor)
	{
		return;
	}

	if (!bShouldRotate)
	{
		return;
	}

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	// Z 값만 바꿔주면되니까 나머지는 0
	GetOwner()->SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f));
}

