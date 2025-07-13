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
	// 포션 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PotionQuantity = 10;

	// 포션 마실 때 회복량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PotionHealAmount = 15.f;

	// 포션 마시고 Actor 지워줌
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASPRPotion> PotionClass;

	// 지워줘야하니까 소켓이름도 알아야한다.
	UPROPERTY(EditAnywhere)
	FName PotionSocketName;

	// 포션 병을 스폰해서 손에 쥐어줄 때 저장할 포션 변수
	class ASPRPotion* PotionActor;

public:	
	
	USPRPotionInventoryComponent();

	void DrinkPotion();

	void SpawnPotion();

	void DespawnPotion();

	void SetPotionQuantity(int32 InQuantity);

	FORCEINLINE int GetPotionQuantity() const { return PotionQuantity; }
	
	// 포션 수량 델리게이트함수
	void BroadcastPotionUpdate();
};
