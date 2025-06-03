// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRStateComponent.h"
#include "Kismet/KismetSystemLibrary.h"

USPRStateComponent::USPRStateComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void USPRStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USPRStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void USPRStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		//딜레이 타임 후 활성화
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		// 꼭 UFUCTION으로 함수를 만들어줘야함
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;

		// 3초뒤에 활성화 Duration을 줄때 3초안에 여러번 실행되면 Duration이 초기화되면서 계속 미뤄질수있는데 이러한걸 방지
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentAction);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}


void USPRStateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;
	ClearState();
}

void USPRStateComponent::ClearState()
{
	CurrentState = FGameplayTag::EmptyTag;
}

bool USPRStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const
{
	// 밑의 코드 풀이
	// if (CurrentState == "Attacking" || CurrentState == "Rolling")
	// {
	//		return true;
	// }
	return TagsToCheck.HasTagExact(CurrentState);
}

