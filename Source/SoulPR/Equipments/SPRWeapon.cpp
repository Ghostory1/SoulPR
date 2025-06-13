// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRWeapon.h"

#include "Components/SPRCombatComponent.h"
#include "Data/SPRMontageActionData.h"
#include "SPRGameplayTags.h"
#include "Components/SPRWeaponCollisionComponent.h"
#include "Kismet/GameplayStatics.h"
ASPRWeapon::ASPRWeapon()
{
	WeaponCollision = CreateDefaultSubobject<USPRWeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	StaminaCostMap.Add(SPRGameplayTags::Character_Attack_Light, 7.f);
	StaminaCostMap.Add(SPRGameplayTags::Character_Attack_Running, 12.f);
	StaminaCostMap.Add(SPRGameplayTags::Character_Attack_Special, 15.f);
	StaminaCostMap.Add(SPRGameplayTags::Character_Attack_Heavy, 20.f);

	DamageMultiplierMap.Add(SPRGameplayTags::Character_Attack_Heavy, 1.8f);
	DamageMultiplierMap.Add(SPRGameplayTags::Character_Attack_Running, 1.8f);
	DamageMultiplierMap.Add(SPRGameplayTags::Character_Attack_Special, 2.1f);
}

void ASPRWeapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<USPRCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		const FName AttachSocket = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
		AttachToOwner(AttachSocket);

		// 무기의 충돌 트레이스 컴포넌트에 무기 메쉬 컴포넌트를 설정
		WeaponCollision->SetWeaponMesh(Mesh);
		
		// 무기를 소유한 OwnerActor를 충돌에서 무시
		WeaponCollision->AddIgnoredActor(GetOwner());
	}
}

void ASPRWeapon::UnequipItem()
{

}

UAnimMontage* ASPRWeapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

float ASPRWeapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	return 0.f;
}

float ASPRWeapon::GetAttackDamage() const
{
	if (const AActor* OwenerActor = GetOwner())
	{
		const FGameplayTag LastAttackType = CombatComponent->GetLastAttackType();

		if (DamageMultiplierMap.Contains(LastAttackType))
		{
			const float Multiplier = DamageMultiplierMap[LastAttackType];
			return BaseDamage * Multiplier;
		}
	}

	return BaseDamage;
}

void ASPRWeapon::OnHitActor(const FHitResult& Hit)
{
	AActor* TargetActor = Hit.GetActor();

	// 데미지 방향
	FVector DamageDirection = GetOwner()->GetActorForwardVector();

	// 데미지
	float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor, // 누구한테 전달할지
		AttackDamage, // 데미지 값
		DamageDirection, // 데미지 방향
		Hit, // HitResult 값
		GetOwner()->GetInstigatorController(), // 공격자가 누구인지
		this, // 데미지커서 ( 데미지의 원인 , 무기에 의해 발생된거니 this로)
		nullptr // 충돌처리 언제 발생할지 -> Animation Notify로 함
	);
}
