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
			// �ָ��� ����� ����ϱ� ������ ĳ������ ���̷�Ż�޽��� �����ش�.
			// MainCollision
			WeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());
			//SecondCollision
			SecondWeaponCollision->SetWeaponMesh(OwnerCharacter->GetMesh());

			// �ָ��� �����̱⶧���� ���⼭ ������ �ش�.
			// ������ �����Ǵ� ������� �ִϸ��̼��� Notify�� ó��
			CombatComponent->SetCombatEnabled(true);

			// ������ ������ CombatType���� ������Ʈ
			if (USPRAnimInstance* Anim = Cast<USPRAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance()))
			{
				Anim->UpdateCombatMode(CombatType);
			}

			// ���⸦ ������ OwnerActor�� �浹���� ������
			WeaponCollision->AddIgnoredActor(OwnerCharacter);
			SecondWeaponCollision->AddIgnoredActor(OwnerCharacter);
		}
	}
}
