// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
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

	// 전투 활성화 상태인지?
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	// 마지막 AttackType
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;
public:
	void SetWeapon(ASPRWeapon* NewWeapon);
	FORCEINLINE ASPRWeapon* GetMainWeapon() const { return MainWeapon; }
	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE void SetCombatEnabled(const bool bEnabled) { bCombatEnabled = bEnabled; }

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; };
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; };

	
};
