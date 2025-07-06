// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "SPRDefine.h"
#include "AnimNotify_SPREquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API UAnimNotify_SPREquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

protected:
	// �������� �˾ƺ��� �±�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MontageActionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESPREquipmentType EquipmentType = ESPREquipmentType::Weapon;

public:
	UAnimNotify_SPREquipWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
