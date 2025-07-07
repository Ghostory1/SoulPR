// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPRDefine.h"
#include "SPRAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SOULPR_API USPRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="References")
	ACharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UCharacterMovementComponent* MovementComponent;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement Data")
	FVector Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float GroundSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bShouldMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	bool bIsFalling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Data")
	float Direction;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	bool bCombatEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	ECombatType CombatType = ECombatType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Data")
	bool bShouldBlocking = false;
public:
	USPRAnimInstance();

	// 애님 인스턴스가 초기화될때 한번 호출
	virtual void NativeInitializeAnimation() override;
	//매 틱마다 호출되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	// 구르기 후 입력 리셋
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetState();


// Animation
public:
	void UpdateCombatMode(const ECombatType InCombatType);
	void UpdateBlocking(bool InShouldBlocking);
// Delegate Functions
protected:
	void OnChangedCombat(const bool bInCombatEnabled);
};
