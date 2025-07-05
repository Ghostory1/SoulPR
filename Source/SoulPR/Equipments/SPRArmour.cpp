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
		
		//Equipment에서 OnConstruct에서 이미 StaticMesh 가 있으니 삭제
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

		// Name_None : 특정 소켓에 붙이는게 아니라서 None으로 설정
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
		// Attach 하던걸 nullptr을 넣어주면 해제됌 
		SkeletalMesh->SetLeaderPoseComponent(nullptr);
	}

	SetBodyPartActive(true);

	// 현재 Transform 정보를 유지하면서 떼어낸다.
	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
}

void ASPRArmour::AttachToOwner(FName SocketName)
{
	Super::AttachToOwner(SocketName);

	if (USkeletalMeshComponent* SkeletalMesh = Cast<USkeletalMeshComponent>(Mesh))
	{
		if (const ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			// Character->GetMesh() 에서 애니메이션 블루프린트로 애니메이션처리를 하고있는데 그 애니메이션을 장비아이템이 따라가게 설정
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
