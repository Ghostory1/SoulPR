// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPRArmour.h"
#include "GameFramework/Character.h"
#include "Components/SPRCombatComponent.h"
#include "Components/SPRAttributeComponent.h"
#include "Character/SPRCharacter.h"

ASPRArmour::ASPRArmour()
{
	if (USkeletalMeshComponent* SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmourMesh"))
	{
		
		//Equipment���� OnConstruct���� �̹� StaticMesh �� ������ ����
		Mesh->DestroyComponent();
		Mesh = SkeletalMesh;
		RootComponent = Mesh;
	}
}

void ASPRArmour::EquipItem()
{
	Super::EquipItem();

	if (USPRCombatComponent* CombatComponent = GetOwner()->GetComponentByClass<USPRCombatComponent>())
	{
		CombatComponent->SetArmour(this);

		// Name_None : Ư�� ���Ͽ� ���̴°� �ƴ϶� None���� ����
		AttachToOwner(NAME_None);
		SetBodyPartActive(false);
	}

	if (USPRAttributeComponent* AttributeComponent = GetOwner()->GetComponentByClass<USPRAttributeComponent>())
	{
		AttributeComponent->IncreaseDefense(DefenseAmount);
	}
}

void ASPRArmour::UnequipItem()
{
	Super::UnequipItem();

	if (USPRAttributeComponent* AttributeComponent = GetOwner()->GetComponentByClass<USPRAttributeComponent>())
	{
		AttributeComponent->DecreaseDefense(DefenseAmount);
	}

	if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh))
	{
		// Attach �ϴ��� nullptr�� �־��ָ� ������ 
		SkeletalMesh->SetLeaderPoseComponent(nullptr);
	}

	SetBodyPartActive(true);

	// ���� Transform ������ �����ϸ鼭 �����.
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

void ASPRArmour::AttachToOwner(FName SocketName)
{
	Super::AttachToOwner(SocketName);

	if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh))
	{
		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			// Character->GetMesh() ���� �ִϸ��̼� �������Ʈ�� �ִϸ��̼�ó���� �ϰ��ִµ� �� �ִϸ��̼��� ���������� ���󰡰� ����
			SkeletalMesh->SetLeaderPoseComponent(Character->GetMesh());
		}
	}
}

void ASPRArmour::SetBodyPartActive(const bool bActive) const
{
	if (const ASPRCharacter* Character = Cast<ASPRCharacter>(GetOwner()))
	{
		Character->SetBodyPartActive(ArmourType, bActive);
	}
}
