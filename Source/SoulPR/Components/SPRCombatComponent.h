// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SPRDefine.h"
#include "Equipments/SPRArmour.h"
#include "SPRCombatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateOnChangedCombat, bool);
DECLARE_MULTICAST_DELEGATE(FDelegateOnChangedWeapon);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Combat 상태 전환을 알리는 Delegate
	FDelegateOnChangedCombat OnChangedCombat;

	// 무기 변경을 알리는 Delegate
	FDelegateOnChangedWeapon OnChangedWeapon;
public:
	USPRCombatComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	class ASPRWeapon* MainWeapon;

	UPROPERTY()
	class ASPRShield* Shield;

	UPROPERTY()
	TMap<ESPRArmourType, ASPRArmour*> ArmourMap;

	// 전투 활성화 상태인지?
	UPROPERTY(EditAnywhere)
	bool bCombatEnabled = false;

	// 방어 자세를 취하고 있는지?
	UPROPERTY()
	bool bBlockingEnabled = false;

	// 마지막 AttackType
	UPROPERTY(VisibleAnywhere)
	FGameplayTag LastAttackType;
public:
	void SetWeapon(ASPRWeapon* NewWeapon);
	void SetArmour(ASPRArmour* NewArmour);
	void SetShield(ASPRShield* NewShield);
	FORCEINLINE ASPRWeapon* GetMainWeapon() const { return MainWeapon; }
	FORCEINLINE ASPRArmour* GetArmour(const ESPRArmourType ArmourType)
	{
		if (ArmourMap.Contains(ArmourType))
		{
			return ArmourMap[ArmourType];
		}
		return nullptr;
	}
	FORCEINLINE ASPRShield* GetShield() const { return Shield; }

	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	void SetCombatEnabled(const bool bEnabled);

	FORCEINLINE bool IsBlockingEnabled() const { return bBlockingEnabled; }
	FORCEINLINE void SetBlockingEnabled(const bool bEnabled) { bBlockingEnabled = bEnabled; }

	FORCEINLINE FGameplayTag GetLastAttackType() const { return LastAttackType; };
	FORCEINLINE void SetLastAttackType(const FGameplayTag& NewAttackTypeTag) { LastAttackType = NewAttackTypeTag; };

private:
	void SpawnPickupItem(const AActor* OwnerActor, const TSubclassOf<ASPREquipment>& NewEquipmentClass) const;
};
