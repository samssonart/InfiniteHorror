// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "NPC_Spirit.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "UIWidgetController.h"

// Sets default values
ANPC_Spirit::ANPC_Spirit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HandCollision"));

	if (HandCollision)
	{
		FAttachmentTransformRules const Rules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		HandCollision->AttachToComponent(GetMesh(), Rules, TEXT("Right_Hand_Socket"));
		HandCollision->SetRelativeLocation(HandCollisionOffset);
		HandCollision->OnComponentBeginOverlap.AddDynamic(this, &ANPC_Spirit::OnAttackOverlapBegin);
		HandCollision->OnComponentEndOverlap.AddDynamic(this, &ANPC_Spirit::OnAttackOverlapEnd);
	}
}

// Called when the game starts or when spawned
void ANPC_Spirit::BeginPlay()
{
	Super::BeginPlay();

	PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ANPC_Spirit::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	if (OtherActor != this)
	{
		if (APlayerCharacter* const PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			int newHealth = PlayerCharacter->GetCurrentMentalHealth() - AttackValue;
			PlayerCharacter->SetMentalHealth(newHealth);
		}
	}
}

void ANPC_Spirit::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex)
{
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

void ANPC_Spirit::AttackStart()
{
	HandCollision->SetCollisionProfileName("SpiritHand");
	HandCollision->SetNotifyRigidBodyCollision(true);
}

void ANPC_Spirit::AttackEnd()
{
	HandCollision->SetCollisionProfileName("SpiritHand");
	HandCollision->SetNotifyRigidBodyCollision(false);
}


