// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPRCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SPRStateComponent.h"
#include "SPRGameplayTags.h"
// Sets default values
ASPRCharacter::ASPRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	AttributeComponent = CreateDefaultSubobject<USPRAttributeComponent>(TEXT("Attribute"));
	StateComponent = CreateDefaultSubobject<USPRStateComponent>(TEXT("State"));
	
}

// Called when the game starts or when spawned
void ASPRCharacter::BeginPlay()
{
	Super::BeginPlay();
	// HUD ����
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
	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Cyan, FString::Printf(TEXT("MaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed));
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

