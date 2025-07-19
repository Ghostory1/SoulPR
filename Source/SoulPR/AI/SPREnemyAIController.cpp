// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SPREnemyAIController.h"
#include "SPREnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/SPREnemy.h"
#include "Components/SPRRotationComponent.h"

#include "Character/SPRCharacter.h"



ASPREnemyAIController::ASPREnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

void ASPREnemyAIController::StopUpdateTarget()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	SetTarget(nullptr);
}

void ASPREnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledEnemy = Cast<ASPREnemy>(InPawn);

	RunBehaviorTree(BehaviorTreeAsset);

	// UpdateTarget 타이머 등록
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTarget, 0.1f, true);
}

void ASPREnemyAIController::OnUnPossess()
{
	StopUpdateTarget();
	ControlledEnemy = nullptr;
	Super::OnUnPossess();
}

void ASPREnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerceptionComponent->GetKnownPerceivedActors(nullptr, OutActors);

	ASPRCharacter* PlayerCharacter = Cast<ASPRCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (OutActors.Contains(PlayerCharacter))
	{
		if (!PlayerCharacter->IsDeath())
		{
			SetTarget(PlayerCharacter);
			ControlledEnemy->ToggleHealthBarVisibility(true);
			ControlledEnemy->SeesTarget(PlayerCharacter);
		}
		else
		{
			SetTarget(nullptr);
			ControlledEnemy->ToggleHealthBarVisibility(false);
		}
	}
	else
	{
		SetTarget(nullptr);
		ControlledEnemy->ToggleHealthBarVisibility(false);
	}
}

void ASPREnemyAIController::SetTarget(AActor* NewTarget) const
{
	if (IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("Target"), NewTarget);
	}

	if (IsValid(ControlledEnemy))
	{
		if (USPRRotationComponent* RotationComponent = ControlledEnemy->GetComponentByClass<USPRRotationComponent>())
		{
			RotationComponent->SetTargetActor(NewTarget);
		}
	}
}
