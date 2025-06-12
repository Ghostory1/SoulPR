// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SPRWeaponCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitActor, const FHitResult&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) , meta=(DisplayName ="Weapon Collision"))
class SOULPR_API USPRWeaponCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//���⿡ �ε��� �浹 ���
	FOnHitActor OnHitActor;
protected:
	// ���� ����
	UPROPERTY(EditAnywhere)
	FName TraceStartSocketName;

	// �� ����
	UPROPERTY(EditAnywhere)
	FName TraceEndSocketName;

protected:
	// Sphere ũ��(������)
	UPROPERTY(EditAnywhere)
	float TraceRadius = 20.f;

	// Trace ��� ObjectType
	UPROPERTY(EditAnywhere)
	TArray <TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	// �浹 ó�� ���� Actors
	UPROPERTY(EditAnywhere)
	TArray<AActor*> IgnoredActors;

	// ����� ��ο� Ÿ��
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::ForDuration;

protected:
	// ������ MeshComponent
	// StaticMesh, SkeletalMesh ��
	UPROPERTY()
	UPrimitiveComponent* WeaponMesh;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	bool bIsCollisionEnabled = false;
public:	
	// Sets default values for this component's properties
	USPRWeaponCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TurnOnCollision();

	void TurnOffCollision();

	void SetWeaponMesh(UPrimitiveComponent* MeshComponent);

	void AddIgnoredActor(AActor* Actor);

	void RemoveIgnoredActor(AActor* Actor);

protected:
	bool CanHitActor(AActor* Actor) const;
	void CollisionTrace();
		
};
