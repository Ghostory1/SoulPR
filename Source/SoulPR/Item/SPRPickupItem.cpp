// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SPRPickupItem.h"
#include "Equipments/SPREquipment.h"
#include "SPRDefine.h"
// Sets default values
ASPRPickupItem::ASPRPickupItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupItemMesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ASPRPickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPRPickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPRPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (ASPREquipment* CDO = EquipmentClass->GetDefaultObject<ASPREquipment>())
		{
			//블루 프린트에서 설정한 값 가져오기 ( 메쉬 에셋)
			Mesh->SetStaticMesh(CDO->MeshAsset);
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void ASPRPickupItem::Interact(AActor* InteractionActor)
{
	//GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, TEXT("Hello Interact"));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InteractionActor;

	ASPREquipment* SpawnItem = GetWorld()->SpawnActor<ASPREquipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (SpawnItem)
	{
		SpawnItem->EquipItem();
		Destroy();
	}
}

