// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPRDefine.h"
#include "Components/ActorComponent.h"
#include "SPRAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, ESPRAttributeType, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// ���� ���� Delegate
	FDelegateOnAttributeChanged OnAttributeChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f;

	// ���¹̳� ������ Ÿ�̸� �ڵ�
	FTimerHandle StaminaRegenTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f;
public:	
	// Sets default values for this component's properties
	USPRAttributeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; };
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; };

	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; };
	// ���¹̳��� ������� üũ
	bool CheckHasEnoughStamina(float StaminaCost) const;

	// ���¹̳� ����
	void DecreaseStamina(float StaminaCost);

	// ���¹̳� ������/���� ���
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);

	// ��������Ʈ ��ε� ĳ��Ʈ
	void BroadcastAttributeChanged(ESPRAttributeType InAttributeType) const;
private:
	// ���¹̳� ������ 
	void RegenerateStaminaHandler();
		
};
