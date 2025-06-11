// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SPRAttackFinished.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName= "Attack Finished"))
class SOULPR_API UAnimNotify_SPRAttackFinished : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.f;

public:
	UAnimNotify_SPRAttackFinished(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
