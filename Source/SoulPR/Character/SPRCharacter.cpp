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
		//ETriggerEvent는 내가 계속 누르고있는지 체크
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

