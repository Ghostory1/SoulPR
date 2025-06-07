// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPREquipment.generated.h"

UCLASS()
class SOULPR_API ASPREquipment : public AActor
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, Category="Equipment | Mesh")
	UStaticMesh* MeshAsset;

	UPROPERTY(EditAnywhere, Category = "Equipment | Mesh")
	UStaticMeshComponent* Mesh;
public:	
	// Sets default values for this actor's properties
	ASPREquipment();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EquipItem();

	virtual void UnequipItem();

	virtual void AttachToOwner(FName SocketName);
};
