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

		// ������ �浹 Ʈ���̽� ������Ʈ�� ���� �޽� ������Ʈ�� ����
		WeaponCollision->SetWeaponMesh(Mesh);
		
		// ���⸦ ������ OwnerActor�� �浹���� ����
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

	// ������ ����
	FVector DamageDirection = GetOwner()->GetActorForwardVector();

	// ������
	float AttackDamage = GetAttackDamage();

	UGameplayStatics::ApplyPointDamage(
		TargetActor, // �������� ��������
		AttackDamage, // ������ ��
		DamageDirection, // ������ ����
		Hit, // HitResult ��
		GetOwner()->GetInstigatorController(), // �����ڰ� ��������
		this, // ������Ŀ�� ( �������� ���� , ���⿡ ���� �߻��ȰŴ� this��)
		nullptr // �浹ó�� ���� �߻����� -> Animation Notify�� ��
	);
}
