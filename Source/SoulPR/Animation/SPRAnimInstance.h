// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

public:
	USPRAnimInstance();

	// �ִ� �ν��Ͻ��� �ʱ�ȭ�ɶ� �ѹ� ȣ��
	virtual void NativeInitializeAnimation() override;
	//�� ƽ���� ȣ��Ǵ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	// ������ �� �Է� ����
	UFUNCTION()
	void AnimNotify_ResetMovementInput();

	UFUNCTION()
	void AnimNotify_ResetState();
};
