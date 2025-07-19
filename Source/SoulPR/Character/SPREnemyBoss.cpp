// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPREnemyBoss.h"

#include "Components/WidgetComponent.h"
#include "UI/SPRBossHealthWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

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

	// 보스 사망시 음악 끄기
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

	// 보스 배경음악 재생
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
