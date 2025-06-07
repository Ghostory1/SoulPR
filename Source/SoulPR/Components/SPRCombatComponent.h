// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPRCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USPRCombatComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	class ASPRWeapon* MainWeapon;

public:
	void SetWeapon(ASPRWeapon* NewWeapon);
		
};
