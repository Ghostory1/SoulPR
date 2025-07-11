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
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

// Delegate Func Section
protected:
	virtual void OnDeath();
	void SetDeathState();
	void OnAttributeChanged(ESPRAttributeType AttributeType, float InValue);
	void SetupHealthBar();
protected:
	// 에디터상 무기 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASPRWeapon> DefaultWeaponClass;

protected:
	FTimerHandle ParriedDelayTimerHandle;
	FTimerHandle StunnedDelayTimerHandle;

	// 피격 시 스턴 확률
	UPROPERTY(EditAnywhere)
	int StunnedRate = 0;
protected:
	UPROPERTY(VisibleAnywhere)
	class USPRAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRRotationComponent* RotationComponent;

	// 타겟팅 충돌 체크용
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* TargetingSphereComponent;

	// LockOn UI Widget
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LockOnWidgetComponent;

	// Health Bar Widget 
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthBarWidgetComponent;
	
// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category="Effect")
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

protected:
	// 적 AI 포인트
	// 지점은 에디터에서 설정
	UPROPERTY(EditAnywhere, Category = "AI | Patrol")
	TArray<class ATargetPoint*> PatrolPoints;
	UPROPERTY(VisibleAnywhere, Category = "AI | Patrol")
	int32 PatrolIndex = 0;
protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);




// Interface
public:
	//ISPRTargeting 구현.
	// 타겟팅시 처리할 로직 처리
	virtual void OnTargeted(bool bTargeted) override;
	// 타겟팅 가능한지 체크
	virtual bool CanBeTargeted() override;

	// SPRCombatInterface 구현
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void PerformAttack(struct FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)  override;
	virtual void Parried() override;

	// HP Bar 토글
	void ToggleHealthBarVisibility(bool bVisibility);
public:
	FORCEINLINE class ATargetPoint* GetPatrolPoint()
	{
		return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr;
	}
	FORCEINLINE void IncrementPatrolIndex()
	{
		// 사이즈 넘어가면 다시 0부터
		PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num();
	}
};
