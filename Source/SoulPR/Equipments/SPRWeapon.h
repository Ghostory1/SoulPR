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
	//소켓 이름을 저장할 프로퍼티
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
