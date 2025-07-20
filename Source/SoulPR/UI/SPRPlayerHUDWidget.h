// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPRDefine.h"
#include "Blueprint/UserWidget.h"
#include "SPRPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API USPRPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	//블루프린트에서 만든 위젯을 바인딩할 변수
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	class USPRStatBarWidget* StaminaBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class USPRStatBarWidget* HealthBarWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class USPRPotionWidget* PotionWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class USPRWeaponWidget* ShieldWidget;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class USPRWeaponWidget* WeaponWidget;

	// 무기가 없을 때 빈 이미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* BlankWeaponIcon;

public:
	USPRPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;
protected:
	void OnAttributeChanged(ESPRAttributeType AttributeType, float InValue);

	// PotionInventoryComponent에 델리게이트 연결할 함수
	void OnPotionQuantityChanged(int32 InAmount);

	// FDelegateOnChangedWeapon 델리게이트 함수
	void OnWeaponChanged();
};
