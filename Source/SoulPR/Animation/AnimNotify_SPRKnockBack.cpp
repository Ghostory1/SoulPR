// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SPRKnockBack.h"
#include "AnimNotify_SPRKnockBack.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
void UAnimNotify_SPRKnockBack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const FVector OriginLocation = MeshComp->GetSocketLocation(LocationSocketName);
	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(MeshComp, OriginLocation, DamageRadius, 12, FLinearColor::White, 2.f, 1.f);
	}

	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (!OwnerPawn)
	{
		return;
	}

	AController* OwnerController = OwnerPawn->GetController();
	if (!OwnerController)
	{
		return;
	}

	// �ڱ� �ڽ��� ������ ����
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerPawn);

	// ���� ������ �ڵ�
	// ������ ��, ��ġ, �ݰ�, 
	UGameplayStatics::ApplyRadialDamage(OwnerPawn, BaseDamage, OriginLocation, DamageRadius, nullptr, IgnoreActors, nullptr, OwnerController, false, ECC_Visibility);

}
