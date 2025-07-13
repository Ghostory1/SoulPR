// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPRPotionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API USPRPotionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	class UTextBlock* PotionQuantityText;

public:
	void SetPotionQuantity(const int InAmount);
};
