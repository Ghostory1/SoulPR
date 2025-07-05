// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipments/SPREquipment.h"
#include "SPRDefine.h"
#include "SPRArmour.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API ASPRArmour : public ASPREquipment
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Equipment | Armour")
	ESPRArmourType ArmourType = ESPRArmourType::Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Armour")
	float DefenseAmount = 0.f;

public:
	ASPRArmour();

	virtual void EquipItem() override;

	virtual void UnequipItem() override;

	virtual void AttachToOwner(FName SocketName) override;
	
	FORCEINLINE ESPRArmourType GetArmourType() const { return ArmourType; }

	void SetBodyPartActive(const bool bActive) const;
};
