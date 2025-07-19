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

    APawn* ControlledPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
    if (!ControlledPawn)
        return;

    ASPREnemy* Enemy = Cast<ASPREnemy>(ControlledPawn);
    if (!Enemy)
        return;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsObject(ControlledEnemyKey.SelectedKeyName, Enemy);
    }
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

    ASPREnemy* Enemy = Cast<ASPREnemy>(BlackboardComp->GetValueAsObject(ControlledEnemyKey.SelectedKeyName));
    if (!Enemy)
        return;

    const USPRStateComponent* StateComponent = Enemy->GetComponentByClass<USPRStateComponent>();
    check(StateComponent);

    FGameplayTagContainer CheckTags;
    CheckTags.AddTag(SPRGameplayTags::Character_State_Parried);
    CheckTags.AddTag(SPRGameplayTags::Character_State_Stunned);

    if (StateComponent->IsCurrentStateEqualToAny(CheckTags))
    {
        SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Stunned);
    }
    else
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
        if (IsValid(TargetActor))
        {
            const float Distance = TargetActor->GetDistanceTo(Enemy);

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
            if (Enemy->GetPatrolPoint())
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