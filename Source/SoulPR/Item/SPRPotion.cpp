// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SPRPotion.h"

ASPRPotion::ASPRPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PotionMesh"));
	SetRootComponent(Mesh);
	// ÄÝ¸®Àü ²ô±â
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); 
}

