// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SPRCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
}

// Called when the game starts or when spawned
void ASPRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	}
}

void ASPRCharacter::Move(const FInputActionValue& Values)
{
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

