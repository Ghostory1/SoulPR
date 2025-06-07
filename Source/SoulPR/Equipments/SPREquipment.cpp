// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipments/SPREquipment.h"
#include "GameFramework/Character.h"

// Sets default values
ASPREquipment::ASPREquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

}

// Called when the game starts or when spawned
void ASPREquipment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPREquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPREquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void ASPREquipment::EquipItem()
{
	// 상속받은 곳에서 구현
}

void ASPREquipment::UnequipItem()
{
	// 상속받은 곳에서 구현
}

// #include "GameFramework/Character.h"
void ASPREquipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

