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

		// 무기의 충돌 트레이스 컴포넌트에 무기 메쉬 컴포넌트를 설정
		WeaponCollision->SetWeaponMesh(Mesh);
		
		// 장착한 무기의 CombatType 으로  업데이트
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (USPRAnimInstance* Anim = Cast<USPRAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}
		}
		
		// 무기를 소유한 OwnerActor를 충돌에서 무시
		WeaponCollision->AddIgnoredActor(GetOwner());

		// 방패를 이미 가지고 있는지 체크하고 소켓의 위치를 잡아준다.
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
	// 맨 주먹이 아니면 드랍
	if (CombatType != ECombatType::MeleeFists)
	{
		// 무기를 먼저 떼어내고
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// 콜리전 켜주고
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
	// LookAt 회전값 구하기 ( 현재 Actor가 공격자를 바라보는 회전값)
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());

	// 현재 Actor의 회전값과 LookAt 회전값의 차이를 구하기
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// Z 축 기준의 회전값 차이만을 취함
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
