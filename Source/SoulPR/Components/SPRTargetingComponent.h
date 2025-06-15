// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SPRDefine.h"
#include "SPRTargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// LockOn ��ų �ĺ����� ������ ������ ũ�� (������)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TargetingRadius = 500.f;

	// LockOn�� ī�޶� ȸ�� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FaceLockOnRotationSpeed = 20.f;

	// ����� ��ο� Ÿ��
	// #include "Kismet/KismetSystemLibrary.h"
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	class UCameraComponent* Camera;

	// ���� LockOn�� ���
	UPROPERTY()
	AActor* LockedTargetActor;

	// LockOn ���� ����
	bool bIsLockOn = false;
public:	
	// Sets default values for this component's properties
	USPRTargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// LockedOn On/Off
	void ToggleLockOn();

	// LockOn ��� ����Ī
	void SwitchingLockedOnActor(ESwitchingDirection InDirection);

	// LockOn ����
	FORCEINLINE bool IsLockOn() const { return bIsLockOn; };

protected:
	// LockOn ��ų �ĺ����� ã�´�.
	void FindTargets(TArray<AActor*>& OutTargetingActors) const;

	// ���� LockOn ��ų ����� ã�� �ش�.
	AActor* FindClosestTarget(TArray < AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None) const;

	// ĳ���Ͱ� ī�޶�(ControlRotation)�� ȸ���� ����ȭ �ǵ��� ����
	void OrientCamera() const;

	// ĳ���Ͱ� �̵� �������� ȸ���ϵ��� ����
	void OrientMovement() const;

	// LockedOn ���¿��� ī�޶� ȸ�� ����
	void FaceLockOnActor() const;

	// LockOn
	void LockOnTarget();

	// LockOn ����
	void StopLockOn();
		
};
