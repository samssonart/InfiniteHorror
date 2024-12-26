// Copyright (c) 2024 Samssonart. All rights reserved.


#include "NPC_Spirit.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ANPC_Spirit::ANPC_Spirit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANPC_Spirit::BeginPlay()
{
	Super::BeginPlay();

	PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

// Called every frame
void ANPC_Spirit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsRotating)
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), PlayerActor->GetActorLocation());
		FRotator InterRotation = FMath::RInterpTo(this->GetActorRotation(), LookAtRotation, DeltaTime, RotationSpeed);
		if (InterRotation != this->GetActorRotation())
		{
			this->SetActorRotation(LookAtRotation);
		}
		else
		{
			IsRotating = false;
		}
	}
}

UBehaviorTree* ANPC_Spirit::GetBehaviorTree()
{
	return BehaviourTree;
}

UAnimMontage* ANPC_Spirit::GetAnimationMontage() const
{
	return AnimationMontage;
}

void ANPC_Spirit::Attack()
{
	// Rotate towards player
	if (PlayerActor)
	{
		IsRotating = true;
	}

	if (AnimationMontage)
	{
		PlayAnimMontage(AnimationMontage);
	}
}


