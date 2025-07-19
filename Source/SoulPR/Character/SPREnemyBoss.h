// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SPREnemy.h"
#include "SPREnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API ASPREnemyBoss : public ASPREnemy
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> BossHealthBarWidgetClass;

	UPROPERTY()
	class USPRBossHealthWidget* BossHealthBarWidget;

protected:
	UPROPERTY(EditAnywhere)
	USoundWave* BossMusicAsset;

	UPROPERTY()
	UAudioComponent* BossMusic;

	// �ѹ��� �뷡 ����
	bool bStartedBossMusic = false;

protected:
	virtual void BeginPlay() override;

	virtual void OnDeath() override;

	virtual void SeesTarget(AActor* InTargetActor) override;
	virtual void Tick(float DeltaSeconds) override;


};
