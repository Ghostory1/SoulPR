// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Components/SPRAttributeComponent.h"
#include "UI/SPRPlayerHUDWidget.h"
#include "GameplayTagContainer.h"
#include "Interfaces/SPRCombatInterface.h"

#include "SPRCharacter.generated.h"


UCLASS()
class SOULPR_API ASPRCharacter : public ACharacter , public ISPRCombatInterface
{
	GENERATED_BODY()
public:
	


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

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InteractAction;

	// ���� Ȱ��ȭ / ��Ȱ��ȭ ���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleCombatAction;

	// Attack
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;
	// Heavy Attack
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* HeavyAttackAction;

	// LockedOn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LockOnTargetAction;

	// �������� Ÿ�� ��ȯ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftTargetAction;

	// ���������� Ÿ�� ��ȯ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RightTargetAction;

	//����ڼ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	// �и�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ParryAction;
	// ���� ���ñ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ConsumeAction;

private:
	// ĳ���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRStateComponent* StateComponent;
	//���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRCombatComponent* CombatComponent;

	// LockOn Targeting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRTargetingComponent* TargetingComponent;

	// ���� �κ��丮 ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRPotionInventoryComponent* PotionInventoryComponent;
	
// Body Parts Mesh
// �� ���� �� �Ⱥ��̰� ���� ����
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* TorsoMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* LegsMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* FeetMesh;

protected:
	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY()
	USPRPlayerHUDWidget* PlayerHUDWidget;
protected:
	//�ָ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASPRFistWeapon> FistWeaponClass;


protected:
	// �ǰ� �� ����, ����Ʈ
	UPROPERTY(EditAnywhere, Category="Effect")
	class USoundCue* ImpactSound;

	// ���� ��� ����
	UPROPERTY(EditAnywhere, Category = "Effect")
	class USoundCue* BlockingSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Effect")
	class UParticleSystem* BlockingParticle;
protected:
	// �ִϸ��̼� ��Ÿ�� - ������
	UPROPERTY(EditAnywhere, Category ="Montage")
	UAnimMontage* RollingMontage;

	// ���� ���ñ�
	UPROPERTY(EditAnywhere, Category = "Montage")
	UAnimMontage* DrinkingMontage;
protected:
	// ���� �ӵ�
	UPROPERTY(EditAnywhere, Category="Movement Speed")
	float SprintingSpeed = 750.f;
	// �Ϲ� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Movement Speed")
	float NormalSpeed = 500.f;

	// ����ڼ� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Movement Speed")
	float BlockingSpeed = 200.f;

	UPROPERTY(VisibleAnywhere, Category="Movement Speed")
	bool bSprinting = false;

protected:
	//�޺� ���� ����

	// �޺� ���� �� ?
	bool bComboSequenceRunning = false;
	// �޺� �Է� ���� ? 
	bool bCanComboInput = false;
	// �޺� ī����
	int32 ComboCounter = 0;

	// �޺� �Է� ����
	bool bSavedComboInput = false;

	// �޺� ���� Ÿ�̸� �ڵ�
	FTimerHandle ComboResetTimerHandle;

protected:
	// ���� ��ġ�ϰ��ִ� ���⿡ ���ִ��� üũ
	bool bFacingEnemy = false;

public:
	ASPRCharacter();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	// ��Ʈ�ѷ��� ���� ������ ��Ƽ����
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USPRStateComponent* GetStateComponent() const { return StateComponent; };
	bool IsDeath() const;

	// �ٵ� ���� ���� �״�
	void SetBodyPartActive(const ESPRArmourType ArmourType, const bool bActive) const;

	// ������ ���� �� ó��
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCurser) override;
	void ImpactEffect(const FVector& Location);
	void ShieldBlockingEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker, const ESPRDamageType InDamageType);
	void OnDeath();

protected:

	bool IsMoving() const;
	bool CanToggleCombat() const;
	FORCEINLINE bool IsSprinting() const { return bSprinting; };

	// ��ǲ ���� �Լ�
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	
	// ���� -> �����̽��� ���
	void Sprinting();
	void StopSprint();

	//������ -> �����̽��� ª��
	void Rolling();
	// ��ȣ �ۿ�Ű
	void Interact();

	// ���� ���� ��ȯ
	void ToggleCombat();
	void AutoToggleCombat();

	// Attack
	void Attack();
	void SpecialAttack();
	void HeavyAttack();

	//LockedOn
	void LockOnTarget();
	void LeftTarget();
	void RightTarget();

	// ��� �ڼ�
	void Blocking();
	void BlockingEnd();

	// �и�
	void Parrying();

	// ���� ���ñ�
	void Consume();

protected:
	// ���� ���¿��� ���� ������ �Ϲݰ���
	FGameplayTag GetAttackPerform() const;

	// ���� ���� ���� üũ
	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;

	// ���� ����
	void DoAttack(const FGameplayTag& AttackTypeTag);

	//�޺� ����
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);
	//�޺� �ʱ�ȭ
	void ResetCombo();

	// ��� �ڼ� ���� ����
	bool CanPlayerBlockStance() const;

	// ���� ���� �� ��������?
	bool CanPerformAttackBlocking() const;

	// �и��� ��������?
	bool CanPerformParry() const;

	// �и��� ���� ����
	bool ParriedAttackSucceed() const;

	// ���� ���� �� �ִ� ��Ȳ����
	bool CanDrinkPotion() const;

	// ���� ���ô� ���� ���߿� ��ҵ� �� �Լ� (���� ���ñ� �ߴ�)
	void InterruptWhileDrinkingPotion() const;
public:
	//�޺� AnimNotify ����
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);

//  ISPRCombatInterface �������̽� ������
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
};
