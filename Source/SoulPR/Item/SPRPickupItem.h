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
	//외형 설정
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Interact(AActor* InteractionActor) override;

public:
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<class ASPREquipment>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; };

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class ASPREquipment> EquipmentClass;
};
