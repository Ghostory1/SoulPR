// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/SPRAttributeComponent.h"
#include "SPRBossHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API USPRBossHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	class USPRStatBarWidget* HealthBarWidget;

public:
	void Init(USPRAttributeComponent* AttributeComponent);

protected:
	void OnAttributeChanged(ESPRAttributeType AttributeType, float InValue);
};
