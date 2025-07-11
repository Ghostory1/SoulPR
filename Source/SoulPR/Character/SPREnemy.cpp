// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPREnemy.h"
#include "SPRDefine.h"
#include "GameplayTags.h"
#include "Components/SPRAttributeComponent.h"
#include "Components/SPRStateComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SPRCombatComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "SPRGameplayTags.h"
#include "Equipments/SPRWeapon.h"
#include "UI/SPRStatBarWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISense_Damage.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/SPRRotationComponent.h"
#include "AI/SPREnemyAIController.h"

ASPREnemy::ASPREnemy()
{
 
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement= true;

	//Targeting ��ü ���� �� Collision ����
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphere"));
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// LockOn ����
	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	LockOnWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetVisibility(false);

	// Health ����
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HealthBarWidgetComponent->SetDrawSize(FVector2D(100.f, 5.f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetVisibility(false);


	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<USPRAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<USPRStateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<USPRCombatComponent>(TEXT("Combat"));
	RotationComponent = CreateDefaultSubobject<USPRRotationComponent>(TEXT("RotationComponent"));

	// OnDeath Delegate�� �Լ� ���ε�
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
	AttributeComponent->OnAttributeChanged.AddUObject(this, &ThisClass::OnAttributeChanged);
}


void ASPREnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� ����
	if (DefaultWeaponClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;

		ASPRWeapon* Weapon = GetWorld()->SpawnActor<ASPRWeapon>(DefaultWeaponClass, GetActorTransform(), Params);
		CombatComponent->SetCombatEnabled(true);
		Weapon->EquipItem();
	}

	// ü�¹� ����
	SetupHealthBar();
}


void ASPREnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASPREnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ASPREnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damaged : %f"), ActualDamage));
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// ������ ����
		FVector ShotDirection = PointDamageEvent->ShotDirection;
		// ��Ʈ ��ġ( ǥ�� ���� ����)
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		// ��Ʈ ����
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		// ��Ʈ�� ��ü�� Location( ��ü �߽� ���� )
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		
		// AI �� �������� �ν��Ҽ� �ֵ��� �˷���
		// �������� ������ ������ �˸��� �Լ�
		UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), ActualDamage, HitLocation, HitLocation);

		ImpactEffect(ImpactPoint);
		//���⿡ �´� �ִϸ��̼� ����
		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void ASPREnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(ParriedDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(StunnedDelayTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void ASPREnemy::OnDeath()
{
	//Stop Behavior Tree
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->GetBrainComponent()->StopLogic(TEXT("Death"));
	}

	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Ragdoll
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		MeshComp->SetSimulatePhysics(true);
	}
	// ��� �� HP UI �Ⱥ��̰� ����
	SetDeathState();
}
void ASPREnemy::SetDeathState()
{
	if (StateComponent)
	{
		StateComponent->SetState(SPRGameplayTags::Character_State_Death);
	}

	if (ASPREnemyAIController* AIController = Cast<ASPREnemyAIController>(GetController()))
	{
		AIController->StopUpdateTarget();
	}

	ToggleHealthBarVisibility(false);
}
void ASPREnemy::OnAttributeChanged(ESPRAttributeType AttributeType, float InValue)
{
	if (AttributeType == ESPRAttributeType::Health)
	{
		if (HealthBarWidgetComponent)
		{
			if (const USPRStatBarWidget* StatBar = Cast<USPRStatBarWidget>(HealthBarWidgetComponent->GetWidget()))
			{
				StatBar->SetRatio(InValue);
			}
		}
	}
}

void ASPREnemy::SetupHealthBar()
{
	if (HealthBarWidgetComponent)
	{
		if (USPRStatBarWidget* StatBar = Cast<USPRStatBarWidget>(HealthBarWidgetComponent->GetWidget()))
		{
			// HP Bar ���� ����
			StatBar->FillColorAndOpacity = FLinearColor::Red;
		}
	}

	// ü�� �ʱⰪ ����
	if (AttributeComponent)
	{
		AttributeComponent->BroadcastAttributeChanged(ESPRAttributeType::Health);
	}
}

void ASPREnemy::ImpactEffect(const FVector& Location)
{
	// Sound , ��ƼŬ ó��
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, Location);
	}
}

void ASPREnemy::HitReaction(const AActor* Attacker)
{
	check(CombatComponent);


	// ����
	float StunnedDelay = 0.f;
	if (StunnedRate >= FMath::RandRange(1, 100))
	{
		StateComponent->SetState(SPRGameplayTags::Character_State_Stunned);
		StunnedDelay = FMath::FRandRange(0.5f, 3.f);
	}
	// �ִϸ��̼� ó��
	if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactAnimation(Attacker))
	{
		const float DelaySeconds = PlayAnimMontage(HitReactAnimMontage) + StunnedDelay;

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]()
			{
				FGameplayTagContainer CheckTags;
				CheckTags.AddTag(SPRGameplayTags::Character_State_Stunned);
				if (StateComponent->IsCurrentStateEqualToAny(CheckTags))
				{
					StateComponent->ClearState();
				}
			});
		GetWorld()->GetTimerManager().SetTimer(StunnedDelayTimerHandle, TimerDelegate, DelaySeconds, false);
	}
}



void ASPREnemy::OnTargeted(bool bTargeted)
{
	if (LockOnWidgetComponent)
	{
		LockOnWidgetComponent->SetVisibility(bTargeted);
	}
}


bool ASPREnemy::CanBeTargeted()
{
	// �̹� �׾����� Tagó���ؼ� ���̻� Ÿ���� X
	if (!StateComponent)
	{
		return false;
	}

	FGameplayTagContainer TagCheck;
	TagCheck.AddTag(SPRGameplayTags::Character_State_Death);
	return StateComponent->IsCurrentStateEqualToAny(TagCheck) == false;
}

void ASPREnemy::ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
	}
}

void ASPREnemy::DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->DeactivateCollision(WeaponCollisionType);
	}
}

void ASPREnemy::PerformAttack(FGameplayTag& AttackTypeTag, FOnMontageEnded& MontageEndedDelegate)
{
	// �����ϰ� ���� �ִϸ��̼� ��������
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	// ���Ͻ� ���� �Ұ� 
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Stunned);
	if (StateComponent->IsCurrentStateEqualToAny(CheckTags))
	{
		return;
	}

	if (const ASPRWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(SPRGameplayTags::Character_State_Attacking);
		CombatComponent->SetLastAttackType(AttackTypeTag);
		AttributeComponent->ToggleStaminaRegeneration(false);

		if (UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag))
		{
			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(Montage);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, Montage);
			}
		}

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}
void ASPREnemy::Parried()
{
	check(StateComponent);
	check(CombatComponent);
	
	StopAnimMontage();
	StateComponent->SetState(SPRGameplayTags::Character_State_Parried);

	if (const ASPRWeapon* MainWeapon = CombatComponent->GetMainWeapon())
	{
		UAnimMontage* ParriedAnimMontage = MainWeapon->GetMontageForTag(SPRGameplayTags::Character_Action_ParriedHit);
		
		// �и��� ���ϸ� 1�� ������ ���ؼ� �������� ���ϰ� ����
		const float Delay = PlayAnimMontage(ParriedAnimMontage) + 1.f;
		
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this]
			{
				FGameplayTagContainer CheckTags;
				CheckTags.AddTag(SPRGameplayTags::Character_State_Death);
				if (StateComponent->IsCurrentStateEqualToAny(CheckTags) == false)
				{
					StateComponent->ClearState();
				}
			}
		);
		// ������ ���� false -> Loop �ɼ� (false : �ѹ��� ����)
		GetWorld()->GetTimerManager().SetTimer(ParriedDelayTimerHandle, TimerDelegate, Delay, false);
	}
}

void ASPREnemy::ToggleHealthBarVisibility(bool bVisibility)
{
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(bVisibility);
	}
}

