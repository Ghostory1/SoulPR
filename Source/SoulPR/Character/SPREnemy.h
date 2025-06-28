// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/SPRTargeting.h"
#include "Interfaces/SPRCombatInterface.h"
#include "SPREnemy.generated.h"

UCLASS()
class SOULPR_API ASPREnemy : public ACharacter , public ISPRTargeting, public ISPRCombatInterface
{
	GENERATED_BODY()

public:
	ASPREnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDeath();
protected:
	// �����ͻ� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASPRWeapon> DefaultWeaponClass;
protected:
	UPROPERTY(VisibleAnywhere)
	class USPRAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRCombatComponent* CombatComponent;
	

// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category="Effect")
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

protected:
	// �� AI ����Ʈ
	// ������ �����Ϳ��� ����
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<class ATargetPoint*> PatrolPoints;
	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;
protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);


// Ÿ���� �浹 üũ��
protected:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* TargetingSphereComponent;

// LockOn UI Widget
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LockOnWidgetComponent;

// Interface
public:
	//ISPRTargeting ����.
	// Ÿ���ý� ó���� ���� ó��
	virtual void OnTargeted(bool bTargeted) override;
	// Ÿ���� �������� üũ
	virtual bool CanBeTargeted() override;

	// SPRCombatInterface ����
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void PerformAttack(struct FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)  override;
public:
	FORCEINLINE class ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}
	FORCEINLINE void IncrementPatrolIndex()
	{
		// ������ �Ѿ�� �ٽ� 0����
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}
};
