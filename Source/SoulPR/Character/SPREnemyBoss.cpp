// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPREnemyBoss.h"

#include "Components/WidgetComponent.h"
#include "UI/SPRBossHealthWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	// ���� ����� ���� ����
	if (IsValid(BossMusic))
	{
		BossMusic->FadeOut(2.f, 0);
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

	// ���� ������� ���
	if (BossMusicAsset)
	{
		if (!bStartedBossMusic)
		{
			bStartedBossMusic = true;
			BossMusic = UGameplayStatics::SpawnSound2D(this, BossMusicAsset);
			BossMusic->FadeIn(1.f);
		}
	}
}

void ASPREnemyBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(1, 1.5f, FColor::Cyan, FString::Printf(TEXT("%f"), AttributeComponent->GetBaseStamina()));
}
