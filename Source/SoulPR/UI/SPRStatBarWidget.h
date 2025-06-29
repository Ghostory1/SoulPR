// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPRStatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API USPRStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	class UProgressBar* StatBar;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatBar")
	FLinearColor FillColorAndOpacity = FLinearColor::Red;
public:
	USPRStatBarWidget(const FObjectInitializer& ObjectInitializer);

public:
	virtual void NativePreConstruct() override;
	void SetRatio(float Ratio) const;
};
