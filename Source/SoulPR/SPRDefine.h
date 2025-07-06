// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define COLLISION_OBJECT_INTERACTION ECC_GameTraceChannel1
#define COLLISION_OBJECT_TARGETING ECC_GameTraceChannel2

UENUM(BlueprintType)
enum class ESPRAttributeType : uint8
{
	Stamina,
	Health
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	None,
	Left,
	Right,
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None,
	SwordShield,
	TwoHanded,
	MeleeFists

};

UENUM(BlueprintType)
enum class EWeaponCollisionType : uint8
{
	MainCollision,
	SecondCollision,
};

UENUM(BlueprintType)
enum class ESPRAIBehavior : uint8
{
	Idle,
	Patrol,
	MeleeAttack,
	Approach,
};

UENUM(BlueprintType)
enum class ESPRArmourType: uint8
{
	Chest, // 상의
	Pants, // 바지
	Boots, // 신발 
	Gloves, // 장갑
};

UENUM(BlueprintType)
enum class ESPREquipmentType : uint8
{
	// AnimNotify_EquipWeapon에서 사용
	Weapon,
	Shield,
};