// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SPREnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API ASPREnemyAIController : public AAIController
{

	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviorTreeAsset;

	// AI�� �ֺ�ȯ���� �ν��� �� �ְ� ���ִ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* AIPerceptionComponent;

	FTimerHandle TimerHandle;

	UPROPERTY()
	class ASPREnemy* ControlledEnemy;

public:
	ASPREnemyAIController();

public:
	void StopUpdateTarget();
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	void UpdateTarget() const;
	void SetTarget(AActor* NewTarget) const;
};
