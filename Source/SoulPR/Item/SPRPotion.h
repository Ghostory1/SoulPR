// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPRPotion.generated.h"

UCLASS()
class SOULPR_API ASPRPotion : public AActor
{
	GENERATED_BODY()
	
protected:
	// ¿ÜÇü Mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
public:	
	ASPRPotion();

};
