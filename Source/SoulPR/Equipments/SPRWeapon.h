// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	class USPRMontageActionData* MontageActionData;
	
	UPROPERTY()
	class USPRCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRWeaponCollisionComponent* WeaponCollision;

	//각 무기마다 스태미너
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	// 기본 데미지
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;

	// 데미지 승수
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;
public:
	ASPRWeapon();

public:
	virtual void EquipItem() override;
	virtual void UnequipItem() override;

	UAnimMontage* GetMontageForTag(const struct FGameplayTag& Tag, const int32 Index = 0) const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; };
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; };

	float GetStaminaCost(const FGameplayTag& InTag) const;

	float GetAttackDamage() const;
	FORCEINLINE USPRWeaponCollisionComponent* GetCollision() const { return WeaponCollision; };

public:
	// 무기의 Collision에 검출된 Actor에 Damage 전달
	// SPRWeaponCollisionComponent의 Delegate에 전달될 함수
	void OnHitActor(const FHitResult& Hit);
};
