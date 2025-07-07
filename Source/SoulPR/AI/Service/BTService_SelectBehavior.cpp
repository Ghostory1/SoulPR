// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehavior.h"

#include "AIController.h"
#include "Character/SPREnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SPRStateComponent.h"

UBTService_SelectBehavior::UBTService_SelectBehavior()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_SelectBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	
	ControlledEnemy = Cast<ASPREnemy>(ControlledPawn);
}

void UBTService_SelectBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* Nodememory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, Nodememory, DeltaSeconds);

	UpdateBehavior(OwnerComp.GetBlackboardComponent());
}

void UBTService_SelectBehavior::SetBehaviorKey(UBlackboardComponent* BlackboardComp, ESPRAIBehavior Behavior) const
{
	BlackboardComp->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<uint8>(Behavior));
}

void UBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* BlackboardComp) const
{
	check(BlackboardComp);
	check(ControlledEnemy);
	
	
	const USPRStateComponent* StateComponent = ControlledEnemy->GetComponentByClass<USPRStateComponent>();
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Parried);

	// �̹� ���� ����
	if (StateComponent->IsCurrentStateEqualToAny(CheckTags))
	{
		SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Stunned);
	}
	else
	{
		// ���� ���¿��� ������������

		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

		if (IsValid(TargetActor))
		{
			const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

			// ���ݹ��� ����
			if (Distance <= AttackRangeDistance)
			{
				SetBehaviorKey(BlackboardComp, ESPRAIBehavior::MeleeAttack);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Approach);
			}
		}
		else
		{
			// Patrol Point�� ������
			if (ControlledEnemy->GetPatrolPoint() != nullptr)
			{
				SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Patrol);
			}
			else
			{
				SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Idle);
			}
		}

	}

}
