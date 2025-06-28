// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SPRDefine.h"
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

	//Combat Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	ECombatType CombatType = ECombatType::SwordShield;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment | Animation")
	class USPRMontageActionData* MontageActionData;
	
	UPROPERTY()
	class USPRCombatComponent* CombatComponent;


	// Collision Ÿ��
	UPROPERTY(VisibleAnywhere)
	class USPRWeaponCollisionComponent* WeaponCollision;

	// �� SecondWeaponCollision �� �ʿ��Ѱ� ? 
	// �ְ��̳� ���ʼտ� ���Ⱑ �ִ°���� ó��
	UPROPERTY(VisibleAnywhere)
	class USPRWeaponCollisionComponent* SecondWeaponCollision;

	//�� ���⸶�� ���¹̳�
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> StaminaCostMap;

	// �⺻ ������
	UPROPERTY(EditAnywhere)
	float BaseDamage = 15.f;

	// ������ �¼�
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> DamageMultiplierMap;
public:
	ASPRWeapon();

public:
	virtual void EquipItem() override;
	virtual void UnequipItem() override;

	UAnimMontage* GetMontageForTag(const struct FGameplayTag& Tag, const int32 Index = 0) const;
	UAnimMontage* GetRandomMontageForTag(const struct FGameplayTag& Tag) const;
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;


	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; };
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; };

	float GetStaminaCost(const FGameplayTag& InTag) const;

	float GetAttackDamage() const;
	FORCEINLINE USPRWeaponCollisionComponent* GetCollision() const { return WeaponCollision; };
	FORCEINLINE ECombatType GetCombatType() const { return CombatType; };

public:
	// MainWeapon SecondWeaponCollision ó�� ��� Ű�� ����
	virtual void ActivateCollision(EWeaponCollisionType InCollisionType);
	virtual void DeactivateCollision(EWeaponCollisionType InCollisionType);
public:
	// ������ Collision�� ����� Actor�� Damage ����
	// SPRWeaponCollisionComponent�� Delegate�� ���޵� �Լ�
	void OnHitActor(const FHitResult& Hit);
};
