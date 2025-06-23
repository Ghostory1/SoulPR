// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/SPRWeapon.h"
#include "SPRFistWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API ASPRFistWeapon : public ASPRWeapon
{
	GENERATED_BODY()
public:
	ASPRFistWeapon();

public:
	virtual void EquipItem() override;
};
