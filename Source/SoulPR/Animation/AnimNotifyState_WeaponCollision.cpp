// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_WeaponCollision.h"

#include "Components/SPRCombatComponent.h"
#include "Components/SPRWeaponCollisionComponent.h"
#include "Equipments/SPRWeapon.h"

UAnimNotifyState_WeaponCollision::UAnimNotifyState_WeaponCollision(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UAnimNotifyState_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const USPRCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<USPRCombatComponent>())
		{
			const ASPRWeapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOnCollision();
			}
		}
	}
}

void UAnimNotifyState_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (const USPRCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<USPRCombatComponent>())
		{
			const ASPRWeapon* Weapon = CombatComponent->GetMainWeapon();
			if (::IsValid(Weapon))
			{
				Weapon->GetCollision()->TurnOffCollision();
			}
		}
	}
}
