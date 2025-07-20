// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPRWeaponWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API USPRWeaponWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	class UImage* WeaponImage;
	
public:
	void SetWeaponImage(UTexture2D* InTexture) const;
};
