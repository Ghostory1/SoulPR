// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPRPotionWidget.h"
#include "SPRPotionWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void USPRPotionWidget::SetPotionQuantity(const int InAmount)
{
	PotionQuantityText->SetText(UKismetTextLibrary::Conv_IntToText(InAmount));
}
