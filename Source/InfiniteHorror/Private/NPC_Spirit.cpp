// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "NPC_Spirit.h"
#include "NPCFactory.h"
#include "PlayerCharacter.h"
#include "NPCDissolveLatentAction.h"
#include "GameFramework/Actor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "UIWidgetController.h"
#include "Materials/MaterialParameterCollectionInstance.h"

TObjectPtr<UObject> WorldContextObj;

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

	WorldContextObj = GEngine->GetWorld();

	PlayerActor = UGameplayStatics::GetPlayerCharacter(WorldContextObj, 0);

	if (MaterialParameterCollection)
	{
		MPC_Instance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	}

	if (MPC_Instance)
	{
		MPC_Instance->SetScalarParameterValue(FName("DissolveAmount"), 0.0f);
	}
	
	NPCFactoryRef = Cast<ANPCFactory>(UGameplayStatics::GetActorOfClass(GetWorld(), ANPCFactory::StaticClass()));
}

void ANPC_Spirit::Destroyed()
{
	Super::Destroyed();
	if (!NPCFactoryRef)
	{
		NPCFactoryRef = Cast<ANPCFactory>(UGameplayStatics::GetActorOfClass(GetWorld(), ANPCFactory::StaticClass()));
	}
	
	if (NPCFactoryRef)
	{
		NPCFactoryRef->ResetSpawnState();
	}
}

void ANPC_Spirit::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex, bool const FromSweep, FHitResult const& SweepResult)
{
	if (OtherActor != this && bIsNPCInAttackMode)
	{
		if (APlayerCharacter* const PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			int newHealth = PlayerCharacter->GetCurrentMentalHealth() - AttackValue;
			newHealth = FMath::Max(newHealth, 0);
			PlayerCharacter->SetMentalHealth(newHealth);
		}
	}
}

void ANPC_Spirit::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, int const OtherBodyIndex)
{
	if (WidgetController)
	{
		WidgetController->ResetVisibility(EWidgetType::MentalHealth);
	}
	else
	{
		// Get all UI widgets
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUIWidgetController::StaticClass(), false);

		if (FoundWidgets.Num() > 0)
		{
			WidgetController = Cast<UUIWidgetController>(FoundWidgets[0]);
			if (WidgetController)
			{
				WidgetController->ResetVisibility(EWidgetType::MentalHealth);
			}
		}
	}
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
	bIsNPCInAttackMode = true;
	HandCollision->SetCollisionProfileName("SpiritHand");
	HandCollision->SetNotifyRigidBodyCollision(true);
}

void ANPC_Spirit::AttackEnd()
{
	HandCollision->SetCollisionProfileName("SpiritHand");
	HandCollision->SetNotifyRigidBodyCollision(false);
	bIsNPCInAttackMode = false;
	bHasNPCAttacked = true;
}

void ANPC_Spirit::StartDissolve()
{
	if (!bIsNPCDissolving)
	{
		bIsNPCDissolving = true;
		int id = FMath::RandRange(0, 1000000);
		GetWorld()->GetLatentActionManager().AddNewAction(this, id,
        		new NPCDissolveLatentAction(id, 5.0f, GetWorld()->DeltaTimeSeconds, this));
	}
	
}

void ANPC_Spirit::SetDissolveAmount(float const DissolveAmount)
{
	if (MPC_Instance)
	{
		//bool setSucceeded =
		MPC_Instance->SetScalarParameterValue(FName("DissolveAmount"), DissolveAmount);
		// if (setSucceeded)
		// {
		// 	float currentDAValue;
		// 	MPC_Instance->GetScalarParameterValue(FName("DissolveAmount"), currentDAValue);
		// 	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
		// 		FString::Printf(TEXT("DissolveAmount is now %f"),currentDAValue));
		// }
	}

	if (DissolveAmount >= 1.0f)
	{
		this->Destroy();
	}
}


