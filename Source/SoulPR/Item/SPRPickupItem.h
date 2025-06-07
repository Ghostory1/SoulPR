// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SPRInteract.h"
#include "SPRPickupItem.generated.h"

UCLASS()
class SOULPR_API ASPRPickupItem : public AActor, public ISPRInteract
{
	GENERATED_BODY()
	
public:	
	ASPRPickupItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* Mesh;

};
