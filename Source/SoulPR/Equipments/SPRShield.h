// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/SPREquipment.h"
#include "SPRShield.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API ASPRShield : public ASPREquipment
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UnequipSocketName;

public:
	virtual void EquipItem() override;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }
};
