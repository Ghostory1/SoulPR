// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/SPREquipment.h"
#include "SPRWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API ASPRWeapon : public ASPREquipment
{
	GENERATED_BODY()
protected:
	//���� �̸��� ������ ������Ƽ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName EquipSocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Socket")
	FName UnequipSocketName;
	
	UPROPERTY()
	class USPRCombatComponent* CombatComponent;
public:
	ASPRWeapon();

public:
	virtual void EquipItem() override;
	virtual void UnequipItem() override;
};
