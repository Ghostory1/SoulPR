// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/SPRTargeting.h"
#include "SPREnemy.generated.h"

UCLASS()
class SOULPR_API ASPREnemy : public ACharacter , public ISPRTargeting
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
	UPROPERTY(VisibleAnywhere)
	class USPRAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	class USPRStateComponent* StateComponent;


// Effect Section
protected:
	UPROPERTY(EditAnywhere, Category="Effect")
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	UParticleSystem* ImpactParticle;

// Montage Section
protected:
	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimFront;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimBack;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimLeft;

	UPROPERTY(EditAnywhere, Category = "Montage | HitReact")
	UAnimMontage* HitReactAnimRight;

protected:
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	UAnimMontage* GetHitReactAnimation(const AActor* Attacker) const;


// Ÿ���� �浹 üũ��
protected:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* TargetingSphereComponent;

// LockOn UI Widget
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LockOnWidgetComponent;

// Targeting Interface
public:
	//ISPRTargeting ����.
	// Ÿ���ý� ó���� ���� ó��
	virtual void OnTargeted(bool bTargeted) override;
	// Ÿ���� �������� üũ
	virtual bool CanBeTargeted() override;
};
