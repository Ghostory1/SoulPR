// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Components/SPRAttributeComponent.h"
#include "UI/SPRPlayerHUDWidget.h"
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
	
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	class UInputAction* SprintRollingAction;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRStateComponent* StateComponent;

protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY()
	USPRPlayerHUDWidget* PlayerHUDWidget;

protected:
	// 애니메이션 몽타주 - 구르기
	UPROPERTY(EditAnywhere, Category ="Montage")
	UAnimMontage* RollingMontage;
protected:
	// 질주 속도
	UPROPERTY(EditAnywhere, Category="Sprinting")
	float SprintingSpeed = 750.f;
	// 일반 속도
	UPROPERTY(EditAnywhere, Category = "Sprinting")
	float NormalSpeed = 500.f;
public:
	ASPRCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	// 컨트롤러가 빙의 됐을때 노티파이
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USPRStateComponent* GetStateComponent() const { return StateComponent; };

protected:

	bool IsMoving() const;
	// 인풋 맵핑 함수
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	
	// 질주 -> 스페이스바 길게
	void Sprinting();
	void StopSprint();

	//구르기 -> 스페이스바 짧게
	void Rolling();
};
