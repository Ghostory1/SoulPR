// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPREnemyBoss.h"

#include "Components/WidgetComponent.h"
#include "UI/SPRBossHealthWidget.h"

void ASPREnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	// ü�¹� ������Ʈ ����
	// ������ ������
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->DestroyComponent();
	}

	// ���� ü�¹� ����
	if (BossHealthBarWidgetClass)
	{
		BossHealthBarWidget = CreateWidget<USPRBossHealthWidget>(GetWorld(), BossHealthBarWidgetClass);
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->Init(AttributeComponent);
			BossHealthBarWidget->AddToViewport();
			BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ASPREnemyBoss::OnDeath()
{
	Super::OnDeath();

	if (BossHealthBarWidget)
	{
		BossHealthBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ASPREnemyBoss::SeesTarget(AActor* InTargetActor)
{
	if (IsValid(InTargetActor))
	{
		if (BossHealthBarWidget)
		{
			BossHealthBarWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
}
