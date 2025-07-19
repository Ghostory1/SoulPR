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
	// 서비스가 활성화될 때 실행되는 함수
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
		//AI의 시선을 플레이어에게 고정
		OwnerController->SetFocus(TargetActor, EAIFocusPriority::Gameplay);

		if (const ACharacter* Character = Cast<ACharacter>(ControlledPawn))
		{
			if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
			{
				// 위에서 OwnerController->SetFocus 로 컨트롤러로 고정시켜놔서 그에 따른 설정 -> Controller Rotation을 따르고, Orient는 false 로 꺼줌
				MovementComp->bUseControllerDesiredRotation = true;
				MovementComp->bOrientRotationToMovement = false;
			}
		}
	}
}

void UBTService_Strafe::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 서비스가 비활성화될 때 실행되는 함수
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

	// 시선 고정 해제
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
	// AI 캐릭터의 현재 위치 저장 Origin
	const FVector Origin = ControlledPawn->GetActorLocation();

	// 밑에코드에서 랜덤위치 저장할 변수
	FNavLocation OutRandomPoint(Origin);

	// 네비게이션 시스템 사용하려면 모듈 추가 필요 - Bulid.cs 에서 NavigationSystem 추가
	if (const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(ControlledPawn))
	{
		// 현재 위치에서 , 네비게이션 시스템으로 우리가 설정한 반경안에 랜덤위치를 아웃풋 파라미터 변수로 저장
		if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, OutRandomPoint))
		{
			// 저장된 랜덤위치로 이동
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(OwnerController, OutRandomPoint.Location);
		}
	}
}
