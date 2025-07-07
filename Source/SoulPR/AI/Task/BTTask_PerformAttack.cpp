// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_PerformAttack.h"

#include "AIController.h"
#include "Interfaces/SPRCombatInterface.h"
#include "Components/SPRStateComponent.h"

EBTNodeResult::Type UBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	if (ISPRCombatInterface* CombatInterface = Cast<ISPRCombatInterface>(ControlledPawn))
	{
		FOnMontageEnded MontageEndedDelegate;
		// 델리게이트 바인딩
		MontageEndedDelegate.BindLambda([this, &OwnerComp, ControlledPawn](UAnimMontage* Montage, bool bInterrupted)
			{
				// 몽타주 종료 시 실행될 코드
				UE_LOG(LogTemp, Log, TEXT("Execute MontageEndedDelegate"));

				if (::IsValid(&OwnerComp) == false)
				{
					return;
				}

				if (USPRStateComponent* StateComponent = ControlledPawn->GetComponentByClass<USPRStateComponent>())
				{
					FGameplayTagContainer CheckTags;
					CheckTags.AddTag(SPRGameplayTags::Character_State_Parried);
					if(StateComponent->IsCurrentStateEqualToAny(CheckTags) == false)
					{
						// 패링 상태가 아니면 ClearState
						StateComponent->ClearState();
					}
					
				}
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			});
		// Attack 실행
		CombatInterface->PerformAttack(AttackTypeTag, MontageEndedDelegate);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
