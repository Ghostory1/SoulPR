// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRWeapon.h"

#include "Components/SPRCombatComponent.h"
#include "Data/SPRMontageActionData.h"
#include "SPRGameplayTags.h"
#include "Components/SPRWeaponCollisionComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Animation/SPRAnimInstance.h"
#include "Character/SPRCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "Equipments/SPRShield.h"

ASPRWeapon::ASPRWeapon()
{
	WeaponCollision = CreateDefaultSubobject<USPRWeaponCollisionComponent>("WeaponCollision");
	WeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

	SecondWeaponCollision = CreateDefaultSubobject<USPRWeaponCollisionComponent>("SecondCollision");
	SecondWeaponCollision->OnHitActor.AddUObject(this, &ThisClass::OnHitActor);

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
		
		// ������ ������ CombatType ����  ������Ʈ
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (USPRAnimInstance* Anim = Cast<USPRAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}
		
		// ���⸦ ������ OwnerActor�� �浹���� ����
		WeaponCollision->AddIgnoredActor(GetOwner());

		// ���и� �̹� ������ �ִ��� üũ�ϰ� ������ ��ġ�� ����ش�.
		if (ASPRShield* Shield = CombatComponent->GetShield())
		{
			FName ShieldAttachSocket = Shield->GetUnequipSocketName();

			if (CombatType == ECombatType::SwordShield)
			{
				if (CombatComponent->IsCombatEnabled())
				{
					ShieldAttachSocket = Shield->GetEquipSocketName();
				}
			}

			Shield->AttachToOwner(ShieldAttachSocket);
		}
	}
}
void ASPRWeapon::Drop()
{
	// �� �ָ��� �ƴϸ� ���
	if (CombatType != ECombatType::MeleeFists)
	{
		// ���⸦ ���� �����
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// �ݸ��� ���ְ�
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
	}
}
void ASPRWeapon::UnequipItem()
{

}

UAnimMontage* ASPRWeapon::GetMontageForTag(const FGameplayTag& Tag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(Tag, Index);
}

UAnimMontage* ASPRWeapon::GetRandomMontageForTag(const struct FGameplayTag& Tag) const
{
	return MontageActionData->GetRandomMontageForTag(Tag);
}
float ASPRWeapon::GetStaminaCost(const FGameplayTag& InTag) const
{
	if (StaminaCostMap.Contains(InTag))
	{
		return StaminaCostMap[InTag];
	}
	return 0.f;
}
UAnimMontage* ASPRWeapon::GetHitReactAnimation(const AActor* Attacker) const
{
	// LookAt ȸ���� ���ϱ� ( ���� Actor�� �����ڸ� �ٶ󺸴� ȸ����)
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());

	// ���� Actor�� ȸ������ LookAt ȸ������ ���̸� ���ϱ�
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// Z �� ������ ȸ���� ���̸��� ����
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Back"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Right"));
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = GetMontageForTag(SPRGameplayTags::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Back:
		SelectedMontage = GetMontageForTag(SPRGameplayTags::Character_Action_HitReaction, 1);
		break;
	case EHitDirection::Left:
		SelectedMontage = GetMontageForTag(SPRGameplayTags::Character_Action_HitReaction, 2);
		break;
	case EHitDirection::Right:
		SelectedMontage = GetMontageForTag(SPRGameplayTags::Character_Action_HitReaction, 3);
		break;
	}

	return SelectedMontage;
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

void ASPRWeapon::ActivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOnCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOnCollision();
		break;
	}
}

void ASPRWeapon::DeactivateCollision(EWeaponCollisionType InCollisionType)
{
	switch (InCollisionType)
	{
	case EWeaponCollisionType::MainCollision:
		WeaponCollision->TurnOffCollision();
		break;
	case EWeaponCollisionType::SecondCollision:
		SecondWeaponCollision->TurnOffCollision();
		break;
	}
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
