// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPRAttributeComponent.h"
#include "Components/SPRStateComponent.h"
#include "SPRGameplayTags.h"

// Sets default values for this component's properties
USPRAttributeComponent::USPRAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USPRAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USPRAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool USPRAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void USPRAttributeComponent::DecreaseStamina(float StaminaCost)
{
	BaseStamina = FMath::Clamp(BaseStamina - StaminaCost, 0.f, MaxStamina);
	BroadcastAttributeChanged(ESPRAttributeType::Stamina);
}

void USPRAttributeComponent::ToggleStaminaRegeneration(bool bEnabled, float StartDelay)
{
	if (bEnabled)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRegenTimerHandle) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &ThisClass::RegenerateStaminaHandler, 0.1f, true, StartDelay);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
	}
}

void USPRAttributeComponent::BroadcastAttributeChanged(ESPRAttributeType InAttributeType) const
{
	//델리게이트에 바인딩 되어있는 함수가 있는지 체크 -> 바인딩 되어있는 함수가 없으면 굳이 Broadcast 할 필요는 없다
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.f;
		switch (InAttributeType)
		{
		case ESPRAttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;
		case ESPRAttributeType::Health:
			Ratio = GetHealthRatio();
			break;
		}

		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}

void USPRAttributeComponent::TakeDamageAmount(float DamageAmount)
{
	// 체력 차감
	BaseHealth = FMath::Clamp(BaseHealth - DamageAmount, 0.f, MaxHealth);

	BroadcastAttributeChanged(ESPRAttributeType::Health);

	if (BaseHealth <= 0.f)
	{
		// Call Death Delegate
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}

		// Set Death State
		if (USPRStateComponent* StateComp = GetOwner()->FindComponentByClass<USPRStateComponent>())
		{
			StateComp->SetState(SPRGameplayTags::Character_State_Death);
		}
	}
}

void USPRAttributeComponent::RegenerateStaminaHandler()
{
	BaseStamina = FMath::Clamp(BaseStamina + StaminaRegenRate, 0.f, MaxStamina);
	BroadcastAttributeChanged(ESPRAttributeType::Stamina);
	if (BaseStamina >= MaxStamina)
	{
		ToggleStaminaRegeneration(false);
	}
}

