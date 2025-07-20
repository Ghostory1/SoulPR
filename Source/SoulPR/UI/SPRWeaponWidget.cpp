// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRWeaponWidget.h"

#include "Components/Image.h"
void USPRWeaponWidget::SetWeaponImage(UTexture2D* InTexture) const
{
	WeaponImage->SetBrushFromTexture(InTexture);
}
