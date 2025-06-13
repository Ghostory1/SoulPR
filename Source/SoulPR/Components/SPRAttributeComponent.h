// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPRDefine.h"
#include "Components/ActorComponent.h"
#include "SPRAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FDelegateOnAttributeChanged, ESPRAttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeath)
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

// DELEGATE
public:
	// ���� ���� Delegate
	FDelegateOnAttributeChanged OnAttributeChanged;
	FOnDeath OnDeath;
protected:

	// ���¹̳�
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float BaseStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina = 100.f;

	// ���¹̳� ������ Ÿ�̸� �ڵ�
	FTimerHandle StaminaRegenTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate = 0.2f;

	// ü��
	UPROPERTY(EditAnywhere, Category = "Health")
	float BaseHealth = 100.f;
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.f;
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
	
	FORCEINLINE float GetBaseHealth() const { return BaseHealth; };
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; };

	// ���¹̳��� ������� üũ
	bool CheckHasEnoughStamina(float StaminaCost) const;

	// ���¹̳� ����
	void DecreaseStamina(float StaminaCost);

	// ���¹̳� ������/���� ���
	void ToggleStaminaRegeneration(bool bEnabled, float StartDelay = 2.f);

	// ��������Ʈ ��ε� ĳ��Ʈ
	void BroadcastAttributeChanged(ESPRAttributeType InAttributeType) const;

	void TakeDamageAmount(float DamageAmount);
private:
	// ���¹̳� ������ 
	void RegenerateStaminaHandler();
		
};
