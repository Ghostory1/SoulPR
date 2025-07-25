// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SPRDefine.h"
#include "BTService_SelectBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API UBTService_SelectBehavior : public UBTService
{

	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ControlledEnemyKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere)
	float AttackRangeDistance = 500.f;

public:
	UBTService_SelectBehavior();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds) override;

protected:
	void SetBehaviorKey(UBlackboardComponent* BlackboardComp, ESPRAIBehavior Behavior) const;
	virtual void UpdateBehavior(UBlackboardComponent* BlackboardComp) const;
};
