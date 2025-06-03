// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SPRStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRStateComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	//ĳ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	FGameplayTag CurrentState;

	// ĳ���Ͱ� ��ų ���� Ȧ�� 
	// �̵�Ű �Է� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bMovementInputEnabled = true;
	UFUNCTION()
	void MovementInputEnableAction();

public:	
	USPRStateComponent();

protected:
	
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE void SetState(const FGameplayTag NewState) { CurrentState = NewState; };
	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; };

	//������Ʈ �ʱ�ȭ
	void ClearState();

	// �̵� �Է��� Ȱ��ȭ ��������?
	FORCEINLINE bool MovementInputEnabled() const { return bMovementInputEnabled; };
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);
	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;
		
};
