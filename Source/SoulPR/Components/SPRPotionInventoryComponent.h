// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPRPotionInventoryComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FDelegateOnUpdatePotionAmount, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULPR_API USPRPotionInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FDelegateOnUpdatePotionAmount OnUpdatePotionAmount;

protected:
	// ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PotionQuantity = 10;

	// ���� ���� �� ȸ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PotionHealAmount = 15.f;

	// ���� ���ð� Actor ������
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASPRPotion> PotionClass;

	// ��������ϴϱ� �����̸��� �˾ƾ��Ѵ�.
	UPROPERTY(EditAnywhere)
	FName PotionSocketName;

	// ���� ���� �����ؼ� �տ� ����� �� ������ ���� ����
	class ASPRPotion* PotionActor;

public:	
	
	USPRPotionInventoryComponent();

	void DrinkPotion();

	void SpawnPotion();

	void DespawnPotion();

	void SetPotionQuantity(int32 InQuantity);

	FORCEINLINE int GetPotionQuantity() const { return PotionQuantity; }
	
	// ���� ���� ��������Ʈ�Լ�
	void BroadcastPotionUpdate();
};
