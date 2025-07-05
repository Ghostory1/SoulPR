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

	// 전투 활성화 / 비활성화 토글
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

	// 왼쪽으로 타겟 전환
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftTargetAction;

	// 오른쪽으로 타겟 전환
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* RightTargetAction;

private:
	// 캐릭터 스탯 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRStateComponent* StateComponent;
	//무기 전투 관리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRCombatComponent* CombatComponent;

	// LockOn Targeting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USPRTargetingComponent* TargetingComponent;
	
// Body Parts Mesh
// 방어구 장착 시 안보이게 해줄 부위
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
	//주먹 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASPRFistWeapon> FistWeaponClass;
protected:
	// 피격 시 사운드, 이펙트
	UPROPERTY(EditAnywhere, Category="Effect")
	class USoundCue* ImpactSound;
	UPROPERTY(EditAnywhere, Category = "Effect")
	class UParticleSystem* ImpactParticle;
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

	UPROPERTY(VisibleAnywhere, Category="Sprinting")
	bool bSprinting = false;

protected:
	//콤보 어택 섹션

	// 콤보 진행 중 ?
	bool bComboSequenceRunning = false;
	// 콤보 입력 가능 ? 
	bool bCanComboInput = false;
	// 콤보 카운터
	int32 ComboCounter = 0;

	// 콤보 입력 여부
	bool bSavedComboInput = false;

	// 콤보 리셋 타이머 핸들
	FTimerHandle ComboResetTimerHandle;


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
	bool IsDeath() const;

	// 바디 파츠 껏다 켰다
	void SetBodyPartActive(const ESPRArmourType ArmourType, const bool bActive) const;

	// 데미지 받을 때 처리
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCurser) override;
	void ImpactEffect(const FVector& Location);
	void HitReaction(const AActor* Attacker);
	void OnDeath();

protected:

	bool IsMoving() const;
	bool CanToggleCombat() const;
	FORCEINLINE bool IsSprinting() const { return bSprinting; };

	// 인풋 맵핑 함수
	void Move(const FInputActionValue& Values);
	void Look(const FInputActionValue& Values);
	
	// 질주 -> 스페이스바 길게
	void Sprinting();
	void StopSprint();

	//구르기 -> 스페이스바 짧게
	void Rolling();
	// 상호 작용키
	void Interact();

	// 전투 상태 전환
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

protected:
	// 현재 상태에서 수행 가능한 일반공격
	FGameplayTag GetAttackPerform() const;

	// 공격 가능 조건 체크
	bool CanPerformAttack(const FGameplayTag& AttackTypeTag) const;

	// 공격 실행
	void DoAttack(const FGameplayTag& AttackTypeTag);

	//콤보 실행
	void ExecuteComboAttack(const FGameplayTag& AttackTypeTag);
	//콤보 초기화
	void ResetCombo();

public:
	//콤보 AnimNotify 섹션
	void EnableComboWindow();
	void DisableComboWindow();
	void AttackFinished(const float ComboResetDelay);

//  ISPRCombatInterface 인터페이스 구현부
public:
	virtual void ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
	virtual void DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType) override;
};
