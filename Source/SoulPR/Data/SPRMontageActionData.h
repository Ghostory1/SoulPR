// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SPRMontageActionData.generated.h"


USTRUCT(BlueprintType)
struct FSPRMontageGroup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Animations;
};

/**
 * 
 */
UCLASS()
class SOULPR_API USPRMontageActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Montage Groups")
	TMap<FGameplayTag, FSPRMontageGroup> MontageGroupMap;

public:
	UAnimMontage* GetMontageForTag(const FGameplayTag& GroupTag, const int32 Index);
	
};
