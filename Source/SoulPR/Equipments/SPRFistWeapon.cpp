// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRFistWeapon.h"
#include "SPRFistWeapon.h"

#include "Animation/SPRAnimInstance.h"
#include "Character/SPRCharacter.h"
#include "Components/SPRCombatComponent.h"
#include "Components/SPRWeaponCollisionComponent.h"


ASPRFistWeapon::ASPRFistWeapon()
{

}

void ASPRFistWeapon::EquipItem()
{
	CombatComponent = GetOwner()->GetComponentByClass<USPRCombatComponent>();

	if (CombatComponent)
	{
		CombatComponent->SetWeapon(this);

		if (ASPRCharacter* OwnerCharacter = Cast<ASPRCharacter>(GetOwner()))
		{
			// 주먹을 무기로 사용하기 때문에 캐릭터의 스켈레탈메쉬로 정해준다.
			// MainCollision
			WeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());
			//SecondCollision
			SecondWeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());

			// 주먹이 무기이기때문에 여기서 세팅해 준다.
			// 실제로 장착되는 무기들은 애니메이션의 Notify로 처리
			CombatComponent->SetCombatEnabled(true);

			// 장착한 무기의 CombatType으로 업데이트
			if (USPRAnimInstance* Anim = Cast<USPRAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}

			// 무기를 소유한 OwnerActor를 충돌에서 무시함
			WeaponCollision->AddIgnoredActor(OwnerCharacter);
			SecondWeaponCollision->AddIgnoredActor(OwnerCharacter);
		}
	}
}
