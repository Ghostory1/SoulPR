// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPREnemyBoss.h"

#include "Components/WidgetComponent.h"
#include "UI/SPRBossHealthWidget.h"

void ASPREnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	// 체력바 컴포넌트 삭제
	// 보스는 사용안함
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->DestroyComponent();
	}

	// 보스 체력바 생성
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
