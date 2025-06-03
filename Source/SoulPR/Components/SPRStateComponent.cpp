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
		//������ Ÿ�� �� Ȱ��ȭ
		FLatentActionInfo LatentAction;
		LatentAction.CallbackTarget = this;
		// �� UFUCTION���� �Լ��� ����������
		LatentAction.ExecutionFunction = "MovementInputEnableAction";
		LatentAction.Linkage = 0;
		LatentAction.UUID = 0;

		// 3�ʵڿ� Ȱ��ȭ Duration�� �ٶ� 3�ʾȿ� ������ ����Ǹ� Duration�� �ʱ�ȭ�Ǹ鼭 ��� �̷������ִµ� �̷��Ѱ� ����
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
	// ���� �ڵ� Ǯ��
	// if (CurrentState == "Attacking" || CurrentState == "Rolling")
	// {
	//		return true;
	// }
	return TagsToCheck.HasTagExact(CurrentState);
}

