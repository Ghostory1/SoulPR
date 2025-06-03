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
	//캐릭터의 현재 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	FGameplayTag CurrentState;

	// 캐릭터가 스킬 사용시 홀드 
	// 이동키 입력 상태 관리
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

	//스테이트 초기화
	void ClearState();

	// 이동 입력이 활성화 상태인지?
	FORCEINLINE bool MovementInputEnabled() const { return bMovementInputEnabled; };
	void ToggleMovementInput(bool bEnabled, float Duration = 0.f);
	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& TagsToCheck) const;
		
};
