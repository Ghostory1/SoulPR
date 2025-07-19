// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_SelectBehaviorBoss.h"
#include "Character/SPREnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SPRAttributeComponent.h"


void UBTService_SelectBehaviorBoss::UpdateBehavior(UBlackboardComponent* BlackboardComp) const
{
	check(BlackboardComp);
	ASPREnemy* ControlledEnemy = Cast<ASPREnemy>(BlackboardComp->GetValueAsObject(ControlledEnemyKey.SelectedKeyName));
	check(ControlledEnemy);

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));

	// Ÿ���� ������
	if (IsValid(TargetActor))
	{
		// ���¹̳� üũ
		if (USPRAttributeComponent* AttributeComp = ControlledEnemy->GetComponentByClass<USPRAttributeComponent>())
		{
			// strafe
			if (AttributeComp->GetBaseStamina() <= StaminaCheckValue)
			{
				SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Strafe);
				

				
			}
			else
			{
				const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

				// ���ݹ��� �����̸�
				if (Distance <= AttackRangeDistance)
				{
					SetBehaviorKey(BlackboardComp, ESPRAIBehavior::MeleeAttack);
				}
				else
				{
					SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Approach);
				}
			}
		}
	}
	else
	{
		SetBehaviorKey(BlackboardComp, ESPRAIBehavior::Idle);
	}


}
