// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_Strafe.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTService_Strafe::UBTService_Strafe()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_Strafe::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ���񽺰� Ȱ��ȭ�� �� ����Ǵ� �Լ�
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if(!OwnerController)
	{
		return;
	}

	APawn* ControlledPawn = OwnerController->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	const UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackBoardComp)
	{
		return;
	}

	if (AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject(TargetKey.SelectedKeyName)))
	{
		//AI�� �ü��� �÷��̾�� ����
		OwnerController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);

		if (const ACharacter* Character = Cast<ACharacter>(ControlledPawn))
		{
			if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
			{
				// ������ OwnerController->SetFocus �� ��Ʈ�ѷ��� �������ѳ��� �׿� ���� ���� -> Controller Rotation�� ������, Orient�� false �� ����
				MovementComp->bUseControllerDesiredRotation = true;
				MovementComp->bOrientRotationToMovement = false;
			}
		}
	}
}

void UBTService_Strafe::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// ���񽺰� ��Ȱ��ȭ�� �� ����Ǵ� �Լ�
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (!OwnerController)
	{
		return;
	}

	APawn* ControlledPawn = OwnerController->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	// �ü� ���� ����
	OwnerController->ClearFocus(EAIFocusPriority::Gameplay);

	if (const ACharacter* Character = Cast<ACharacter>(ControlledPawn))
	{
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			MovementComp->bUseControllerDesiredRotation = false;
			MovementComp->bOrientRotationToMovement = true;
		}
	}


	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTService_Strafe::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (!OwnerController)
	{
		return;
	}
	APawn* ControlledPawn = OwnerController->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}
	// AI ĳ������ ���� ��ġ ���� Origin
	const FVector Origin = ControlledPawn->GetActorLocation();

	// �ؿ��ڵ忡�� ������ġ ������ ����
	FNavLocation OutRandomPoint(Origin);

	// �׺���̼� �ý��� ����Ϸ��� ��� �߰� �ʿ� - Bulid.cs ���� NavigationSystem �߰�
	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(ControlledPawn))
	{
		// ���� ��ġ���� , �׺���̼� �ý������� �츮�� ������ �ݰ�ȿ� ������ġ�� �ƿ�ǲ �Ķ���� ������ ����
		if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, OutRandomPoint))
		{
			// ����� ������ġ�� �̵�
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerController, OutRandomPoint.Location);
		}
	}
}
