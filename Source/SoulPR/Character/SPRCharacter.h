// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "SPRCharacter.generated.h"


UCLASS()
class SOULPR_API ASPRCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere)
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	class UInputAction* LookAction;
public:
	ASPRCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	// ��Ʈ�ѷ��� ���� ������ ��Ƽ����
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// ��ǲ ���� �Լ�
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
};
