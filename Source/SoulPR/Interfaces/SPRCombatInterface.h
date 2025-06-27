// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SPRDefine.h"
#include "SPRCombatInterface.generated.h"


UINTERFACE(MinimalAPI)
class USPRCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULPR_API ISPRCombatInterface
{
	GENERATED_BODY()

	
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) = 0;

	// Enemy의 공격 -> Tag와 Montage가 끝나면 호출
	virtual void PerformAttack(struct FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate) {}
};
