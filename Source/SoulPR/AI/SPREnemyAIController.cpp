// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SPREnemyAIController.h"
#include "SPREnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Character/SPREnemy.h"

ASPREnemyAIController::ASPREnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
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
	ControlledEnemy = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	Super::OnUnPossess();
}

void ASPREnemyAIController::UpdateTarget() const
{
	TArray<AActor*> OutActors;
	AIPerceptionComponent->GetKnownPerceivedActors(nullptr, OutActors);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (OutActors.Contains(PlayerCharacter))
	{
		SetTarget(PlayerCharacter);
		ControlledEnemy->ToggleHealthBarVisibility(true);
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
}
