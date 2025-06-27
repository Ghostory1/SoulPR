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


ASPREnemy::ASPREnemy()
{
 
	PrimaryActorTick.bCanEverTick = true;

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

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<USPRAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<USPRStateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<USPRCombatComponent>(TEXT("Combat"));

	// OnDeath Delegate�� �Լ� ���ε�
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);
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

		ImpactEffect(ImpactPoint);

		//���⿡ �´� �ִϸ��̼� ����
		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

void ASPREnemy::OnDeath()
{
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
	// �ִϸ��̼� ó��
	if (UAnimMontage* HitReactAnimMontage = GetHitReactAnimation(Attacker))
	{
		float DelaySeconds = PlayAnimMontage(HitReactAnimMontage);
	}
}

UAnimMontage* ASPREnemy::GetHitReactAnimation(const AActor* Attacker) const
{
	// LookAt ȸ���� ���ϱ� ( ���� Actor�� �����ڸ� �ٶ󺸴� ȸ����)
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	
	// ���� Actor�� ȸ������ LookAt ȸ������ ���̸� ���ϱ�
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	// Z �� ������ ȸ���� ���̸��� ����
	const float DeltaZ = DeltaRotation.Yaw;
	
	EHitDirection HitDirection = EHitDirection::Front;
	
	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Back"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Right"));
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = HitReactAnimFront;
		break;
	case EHitDirection::Back:
		SelectedMontage = HitReactAnimBack;
		break;
	case EHitDirection::Left:
		SelectedMontage = HitReactAnimLeft;
		break;
	case EHitDirection::Right:
		SelectedMontage = HitReactAnimRight;
		break;
	}

	return SelectedMontage;
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
	check(StateComponent)
	check(AttributeComponent)
	check(CombatComponent)

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

