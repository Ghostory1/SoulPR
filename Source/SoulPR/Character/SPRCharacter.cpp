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

ASPRCharacter::ASPRCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 카메라가 움직여도 캐릭터의 방향 유지
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	//캐릭터 기본 속도 설정 , 감속 속도 설정
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-30.f,0.f,0.f));
	// 입력 (마우스)를 돌리면 붐이 돌아가면서 카메라도 움직임
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	//카메라가 직접 컨트롤러 회전을 사용하지 않도록 설정
	// 대신 붐이 이미 회전을 따라가므로, 붐만 따라가게 하면 된다
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<USPRAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<USPRStateComponent>(TEXT("State"));
	CombatComponent = CreateDefaultSubobject<USPRCombatComponent>(TEXT("Combat"));
}

// Called when the game starts or when spawned
void ASPRCharacter::BeginPlay()
{
	Super::BeginPlay();
	// HUD 생성
	if (PlayerHUDWidgetClass)
	{
		PlayerHUDWidget = CreateWidget<USPRPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
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
		//ETriggerEvent는 내가 계속 누르고있는지 체크
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASPRCharacter::Look);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ASPRCharacter::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ASPRCharacter::StopSprint);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ASPRCharacter::Rolling);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASPRCharacter::Interact);
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ASPRCharacter::ToggleCombat);

		//Combat 상태 자동 전환
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AutoToggleCombat);
		// 일반 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ThisClass::Attack);
		// 특수 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::SpecialAttack);
		// Heavy 공격
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);
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

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Attacking);
	CheckTags.AddTag(SPRGameplayTags::Character_State_Rolling);
	CheckTags.AddTag(SPRGameplayTags::Character_State_GeneralAction);
	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ASPRCharacter::Move(const FInputActionValue& Values)
{
	// 이동 입력 가능 상태인지 체크
	check(StateComponent);
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}



	FVector2D MovementVector = Values.Get<FVector2D>();

	// 컨트롤러에서 바라보는 방향 가져오기
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		// Yaw값만 가져오면 된다. 바라보는 방향
		const FRotator YawRotator(0, Rotation.Yaw, 0);
		
		// 
		const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		//주어진 월드 방향 벡터(보통 정규화됨)를 따라 'ScaleValue'만큼 스케일된 이동 입력을 추가한다.
		// ScaleValue가 0보다 작으면, 이동은 반대 방향으로 이루어진다.
		// ScaleValue는 아날로그 입력에 사용될 수 있다
		// 예를 들어, 0.5 같은 정상 값의 절반을 적용하고, -1.0은 방향을 반대로 한다.
		AddMovementInput(ForwardVector, MovementVector.Y);
		AddMovementInput(RightVector, MovementVector.X);

	}
}

void ASPRCharacter::Look(const FInputActionValue& Values)
{
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
		//스태미나 재충전 멈춤
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 이동입력 처리 무시
		StateComponent->ToggleMovementInput(false);

		// 스태미나 차감
		AttributeComponent->DecreaseStamina(15.f);

		// 구르기 애니메이션 재생
		PlayAnimMontage(RollingMontage);


		StateComponent->SetState(SPRGameplayTags::Character_State_Rolling);

		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ASPRCharacter::Interact()
{
	// 물체를 감지하고 상호작용을 할려면 CollisionTrace를 이용
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
				// 토글을 하는 와중에도 토글 불가능
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
	// 무기가 없으면 ToggleCombat으로 무기를 주워 전투 가능하게
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

FGameplayTag ASPRCharacter::GetAttackPerform() const
{
	// 달리기 공격인지 일반공격인지 선택해서 태그를 반환 
	if (IsSprinting())
	{
		return SPRGameplayTags::Character_Attack_Running;
	}
	return SPRGameplayTags::Character_Attack_Light;
}

// 공격이 가능한지 Check
bool ASPRCharacter::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	// 무기를 안들고있으면 공격을 하지못하니 false return
	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(SPRGameplayTags::Character_State_Rolling);
	CheckTags.AddTag(SPRGameplayTags::Character_State_GeneralAction);

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
			// 콤보 한계 도달,
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
			//애니메이션 끝나고 아직 콤보 시퀀스가 유효할 때 -> 추가 입력의 기회 
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
		// 콤보 윈도우가 열려 있을 때 - 최적의 타이밍
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
	//윈도우 끝까지 입력이 없을 시
	
	bCanComboInput = false;

	//만약 입력이 왔다면
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
	// ComboResetDelay 후에 콤보 시퀀스 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}

