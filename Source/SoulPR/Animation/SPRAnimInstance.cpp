// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SPRAnimInstance.h"
#include "SPRAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/SPRCharacter.h"
#include "Components/SPRStateComponent.h"
#include "KismetAnimationLibrary.h"
#include "Components/SPRCombatComponent.h"

USPRAnimInstance::USPRAnimInstance()
{

}
void USPRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());

	if (Character)
	{
		MovementComponent = Character->GetCharacterMovement();
		if (USPRCombatComponent* CombatComponent = Character->GetComponentByClass<USPRCombatComponent>())
		{
			CombatComponent->OnChangedCombat.AddUObject(this, &ThisClass::OnChangedCombat);
		}
	}

}

void USPRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character == nullptr)
	{
		return;
	}
	if (MovementComponent == nullptr)
	{
		return;
	}

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector;

	bIsFalling = MovementComponent->IsFalling();
	
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Character->GetActorRotation());

}

void USPRAnimInstance::AnimNotify_ResetMovementInput()
{
	if (ASPRCharacter* LocalCharactger = Cast<ASPRCharacter>(GetOwningActor()))
	{
		LocalCharactger->GetStateComponent()->ToggleMovementInput(true);
	}
}

void USPRAnimInstance::AnimNotify_ResetState()
{
	// Equip , Unequip이 끝날 때, 기본 노티파이로 Character에서 설정한 GeneralAction 초기화
	if (const ASPRCharacter* LocalCharacter = Cast<ASPRCharacter>(GetOwningActor()))
	{
		LocalCharacter->GetStateComponent()->ClearState();
	}
}

void USPRAnimInstance::UpdateCombatMode(const ECombatType InCombatType)
{
	CombatType = InCombatType;
}

void USPRAnimInstance::UpdateBlocking(bool InShouldBlocking)
{
	bShouldBlocking = InShouldBlocking;
}

void USPRAnimInstance::OnChangedCombat(const bool bInCombatEnabled)
{
	bCombatEnabled = bInCombatEnabled;
}
