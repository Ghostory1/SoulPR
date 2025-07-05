// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPRCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SPRStateComponent.h"
#include "Components/SPRCombatComponent.h"
#include "SPRGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/SPRInteract.h"
#include "Equipments/SPRWeapon.h"
#include "Components/SPRTargetingComponent.h"
#include "Equipments/SPRFistWeapon.h"
#include "Engine/DamageEvents.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ASPRCharacter::ASPRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ī�޶� �������� ĳ������ ���� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	//ĳ���� �⺻ �ӵ� ���� , ���� �ӵ� ����
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-30.f,0.f,0.f));
	// �Է� (���콺)�� ������ ���� ���ư��鼭 ī�޶� ������
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	//ī�޶� ���� ��Ʈ�ѷ� ȸ���� ������� �ʵ��� ����
	// ��� ���� �̹� ȸ���� ���󰡹Ƿ�, �ո� ���󰡰� �ϸ� �ȴ�
	FollowCamera->bUsePawnControlRotation = false;


	// Body Parts
	TorsoMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	TorsoMesh->SetupAttachment(GetMesh());
	LegsMesh->SetupAttachment(GetMesh());
	FeetMesh->SetupAttachment(GetMesh());

	AttributeComponent = CreateDefaultSubobject<USPRAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<USPRStateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<USPRCombatComponent>(TEXT("Combat"));
	TargetingComponent = CreateDefaultSubobject<USPRTargetingComponent>(TEXT("Targeting"));

	// OnDeath Delegate �Լ� ���ε�
	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);


}

// Called when the game starts or when spawned
void ASPRCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Player HUD ����
	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<USPRPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}

	// �ָ� ���� ����
	if (FistWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ASPRFistWeapon* FistWeapon = GetWorld()->SpawnActor<ASPRFistWeapon>(FistWeaponClass, GetActorTransform(), SpawnParams);
		FistWeapon->EquipItem();
	}

}

// Called every frame
void ASPRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed));
	

}
void ASPRCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}
// Called to bind functionality to input
void ASPRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//ETriggerEvent�� ���� ��� �������ִ��� üũ
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASPRCharacter::Look);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ASPRCharacter::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ASPRCharacter::StopSprint);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ASPRCharacter::Rolling);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASPRCharacter::Interact);
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ASPRCharacter::ToggleCombat);

		//Combat ���� �ڵ� ��ȯ
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AutoToggleCombat);
		// �Ϲ� ����
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::Attack);
		// Ư�� ����
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::SpecialAttack);
		// Heavy ����
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);

		// LockOn 
		EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ThisClass::LockOnTarget);
		EnhancedInputComponent->BindAction(LeftTargetAction, ETriggerEvent::Started, this, &ThisClass::LeftTarget);
		EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &ThisClass::RightTarget);
	}
}

bool ASPRCharacter::IsDeath() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Death);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags);
}

void ASPRCharacter::SetBodyPartActive(const ESPRArmourType ArmourType, const bool bActive) const
{
	switch (ArmourType) {
	case ESPRArmourType::Chest:
		TorsoMesh->SetVisibility(bActive);
		TorsoMesh->SetActive(bActive);
		break;
	case ESPRArmourType::Pants:
		LegsMesh->SetVisibility(bActive);
		LegsMesh->SetActive(bActive);
		break;
	case ESPRArmourType::Boots:
		FeetMesh->SetVisibility(bActive);
		FeetMesh->SetActive(bActive);
		break;
	case ESPRArmourType::Gloves:
		break;
	}
}

float ASPRCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCurser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCurser);

	if (AttributeComponent)
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damage : %f"), ActualDamage));
	}

	// ���¸� �ٲٰ� �������� ���ϰ� �Ѵ�.
	StateComponent->SetState(SPRGameplayTags::Character_State_Hit);
	StateComponent->ToggleMovementInput(false);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// ������ ����
		FVector ShotDirection = PointDamageEvent->ShotDirection;
		// ��Ʈ ��ġ ( ǥ�� ���� ����)
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		// ��Ʈ ����
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		// ��Ʈ�� ��ü�� Location( ��ü �߽� ���� )
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);

		HitReaction(EventInstigator->GetPawn());
	}
	return ActualDamage;
}
void ASPRCharacter::ImpactEffect(const FVector& Location)
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
void ASPRCharacter::HitReaction(const AActor* Attacker)
{
	// �ִϸ��̼� ó��
	check(CombatComponent);

	if (UAnimMontage* HitReactAnimMontage = CombatComponent->GetMainWeapon()->GetHitReactAnimation(Attacker))
	{
		PlayAnimMontage(HitReactAnimMontage);
	}
	
}
void ASPRCharacter::OnDeath()
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
bool ASPRCharacter::IsMoving() const
{
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		return MovementComp->Velocity.Size2D() > 3.f && MovementComp->GetCurrentAcceleration() != FVector::Zero();
	}
	
	return false;
}

bool ASPRCharacter::CanToggleCombat() const
{
	check(StateComponent);

	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}
	
	if (CombatComponent->GetMainWeapon()->GetCombatType() == ECombatType::MeleeFists)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(SPRGameplayTags::Character_State_Rolling);
	CheckTags.AddTag(SPRGameplayTags::Character_State_GeneralAction);
	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ASPRCharacter::Move(const FInputActionValue& Values)
{
	// �̵� �Է� ���� �������� üũ
	check(StateComponent);
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}



	FVector2D MovementVector = Values.Get<FVector2D>();

	// ��Ʈ�ѷ����� �ٶ󺸴� ���� ��������
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		// Yaw���� �������� �ȴ�. �ٶ󺸴� ����
		const FRotator YawRotator(0, Rotation.Yaw, 0);
		
		// 
		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		//�־��� ���� ���� ����(���� ����ȭ��)�� ���� 'ScaleValue'��ŭ �����ϵ� �̵� �Է��� �߰��Ѵ�.
		// ScaleValue�� 0���� ������, �̵��� �ݴ� �������� �̷������.
		// ScaleValue�� �Ƴ��α� �Է¿� ���� �� �ִ�
		// ���� ���, 0.5 ���� ���� ���� ������ �����ϰ�, -1.0�� ������ �ݴ�� �Ѵ�.
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);

	}
}

void ASPRCharacter::Look(const FInputActionValue& Values)
{
	// LockOn ���¿����� ���콺 �Է� ����
	if (TargetingComponent && TargetingComponent->IsLockOn())
	{
		return;
	}

	FVector2D LookDirection = Values.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
	}
}

void ASPRCharacter::Sprinting()
{
	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMoving())
	{

		AttributeComponent->ToggleStaminaRegeneration(false);

		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;

		AttributeComponent->DecreaseStamina(0.1f);

		bSprinting = true;
	}
	else
	{
		StopSprint();

	}
}

void ASPRCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
	bSprinting = false;
}

void ASPRCharacter::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);

	if (AttributeComponent->CheckHasEnoughStamina(15.f))
	{
		//���¹̳� ������ ����
		AttributeComponent->ToggleStaminaRegeneration(false);

		// �̵��Է� ó�� ����
		StateComponent->ToggleMovementInput(false);

		// ���¹̳� ����
		AttributeComponent->DecreaseStamina(15.f);

		// ������ �ִϸ��̼� ���
		PlayAnimMontage(RollingMontage);


		StateComponent->SetState(SPRGameplayTags::Character_State_Rolling);

		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ASPRCharacter::Interact()
{
	// ��ü�� �����ϰ� ��ȣ�ۿ��� �ҷ��� CollisionTrace�� �̿�
	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.f;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;

	//#include "Kismet/KismetSystemLibrary.h"
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHit,
		true
	);

	if (bHit)
	{
		if (AActor* HitActor = OutHit.GetActor())
		{
			if (ISPRInteract* Interaction = Cast<ISPRInteract>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}

void ASPRCharacter::ToggleCombat()
{
	check(CombatComponent);
	check(StateComponent);

	if (CombatComponent)
	{
		if (const ASPRWeapon* Weapon = CombatComponent->GetMainWeapon())
		{
			if (CanToggleCombat())
			{
				// ����� �ϴ� ���߿��� ��� �Ұ���
				StateComponent->SetState(SPRGameplayTags::Character_State_GeneralAction);

				if (CombatComponent->IsCombatEnabled())
				{
					PlayAnimMontage(Weapon->GetMontageForTag(SPRGameplayTags::Character_Action_Unequip));
				}
				else
				{
					PlayAnimMontage(Weapon->GetMontageForTag(SPRGameplayTags::Character_Action_Equip));
				}
			}
		}
	}
}

void ASPRCharacter::AutoToggleCombat()
{
	// ���Ⱑ ������ ToggleCombat���� ���⸦ �ֿ� ���� �����ϰ�
	if (CombatComponent)
	{
		if (!CombatComponent->IsCombatEnabled())
		{
			ToggleCombat();
		}
	}
}
void ASPRCharacter::Attack()
{
	const FGameplayTag AttackTypeTag = GetAttackPerform();

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ASPRCharacter::SpecialAttack()
{
	const FGameplayTag AttackTypeTag = SPRGameplayTags::Character_Attack_Special;
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ASPRCharacter::HeavyAttack()
{
	AutoToggleCombat();
	const FGameplayTag AttackTypeTag = SPRGameplayTags::Character_Attack_Heavy;
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ASPRCharacter::LockOnTarget()
{
	TargetingComponent->ToggleLockOn();
}

void ASPRCharacter::LeftTarget()
{
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Left);
}

void ASPRCharacter::RightTarget()
{
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Right);
}


FGameplayTag ASPRCharacter::GetAttackPerform() const
{
	// �޸��� �������� �Ϲݰ������� �����ؼ� �±׸� ��ȯ 
	if (IsSprinting())
	{
		return SPRGameplayTags::Character_Attack_Running;
	}
	return SPRGameplayTags::Character_Attack_Light;
}

// ������ �������� Check
bool ASPRCharacter::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	// ���⸦ �ȵ�������� ������ �������ϴ� false return
	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Rolling);
	CheckTags.AddTag(SPRGameplayTags::Character_State_GeneralAction);
	CheckTags.AddTag(SPRGameplayTags::Character_State_Hit);

	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false
		&& CombatComponent->IsCombatEnabled()
		&& AttributeComponent->CheckHasEnoughStamina(StaminaCost);
}

void ASPRCharacter::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	if (const ASPRWeapon* Weapon = CombatComponent->GetMainWeapon())
	{
		StateComponent->SetState(SPRGameplayTags::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false);
		CombatComponent->SetLastAttackType(AttackTypeTag);

		AttributeComponent->ToggleStaminaRegeneration(false);

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (!Montage)
		{
			// �޺� �Ѱ� ����,
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}

		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		AttributeComponent->DecreaseStamina(StaminaCost);
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ASPRCharacter::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	if (StateComponent->GetCurrentState() != SPRGameplayTags::Character_State_Attacking)
	{
		if (bComboSequenceRunning && bCanComboInput == false)
		{
			//�ִϸ��̼� ������ ���� �޺� �������� ��ȿ�� �� -> �߰� �Է��� ��ȸ 
			ComboCounter++;
			UE_LOG(LogTemp, Warning, TEXT("Additional input : Combo Counter = %d"), ComboCounter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(">>>>> ComboSequence Started  <<<<<<"));
			ResetCombo();
			bComboSequenceRunning = true;
		}
		DoAttack(AttackTypeTag);
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	else if (bCanComboInput)
	{
		// �޺� �����찡 ���� ���� �� - ������ Ÿ�̹�
		bSavedComboInput = true;
	}
}

void ASPRCharacter::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));

	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void ASPRCharacter::EnableComboWindow()
{
	bCanComboInput = true;
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened: Combo Counter = %d"), ComboCounter);
}

void ASPRCharacter::DisableComboWindow()
{
	check(CombatComponent);
	//������ ������ �Է��� ���� ��
	
	bCanComboInput = false;

	//���� �Է��� �Դٸ�
	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		ComboCounter++;
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed: Advancing to next combo = %d"), ComboCounter);
		DoAttack(CombatComponent->GetLastAttackType());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed: No input received"));
	}
}

void ASPRCharacter::AttackFinished(const float ComboResetDelay)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackFinished"));
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true);
	}
	// ComboResetDelay �Ŀ� �޺� ������ ����
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}

void ASPRCharacter::ActivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->ActivateCollision(WeaponCollisionType);
	}
}

void ASPRCharacter::DeactivateWeaponCollision(EWeaponCollisionType WeaponCollisionType)
{
	if (CombatComponent)
	{
		CombatComponent->GetMainWeapon()->DeactivateCollision(WeaponCollisionType);
	}
}

