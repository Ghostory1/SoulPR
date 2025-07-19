// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Service/BTService_SelectBehavior.h"
#include "BTService_SelectBehaviorBoss.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API UBTService_SelectBehaviorBoss : public UBTService_SelectBehavior
{
	GENERATED_BODY()
	
protected:
	virtual void UpdateBehavior(UBlackboardComponent* BlackboardComp)  const override;
};
