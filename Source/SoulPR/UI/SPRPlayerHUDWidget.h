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
	//�������Ʈ���� ���� ������ ���ε��� ����
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

	// ���Ⱑ ���� �� �� �̹���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* BlankWeaponIcon;

public:
	USPRPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void NativeConstruct() override;
protected:
	void OnAttributeChanged(ESPRAttributeType AttributeType, float InValue);

	// PotionInventoryComponent�� ��������Ʈ ������ �Լ�
	void OnPotionQuantityChanged(int32 InAmount);

	// FDelegateOnChangedWeapon ��������Ʈ �Լ�
	void OnWeaponChanged();
};
