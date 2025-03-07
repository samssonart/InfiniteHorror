// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "NPC_Spirit.h"
#include "NPCFactory.h"
#include "PlayerCharacter.h"
#include "NPCDissolveLatentAction.h"
#include "GameFramework/Actor.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
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

	GetComponents<UStaticMeshComponent>(StaticMeshComponents);
}

// Called when the game starts or when spawned
void ANPC_Spirit::BeginPlay()
{
	Super::BeginPlay();

	UObject* WorldContextObj = GetWorld();

	PlayerActor = UGameplayStatics::GetPlayerCharacter(WorldContextObj, 0);
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
			// Assuming you have only one instance or you want the first one found
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
	int id = FMath::RandRange(0, 1000000);
	GetWorld()->GetLatentActionManager().AddNewAction(this, id, new NPCDissolveLatentAction(id, 5.0f, GetWorld()->DeltaTimeSeconds));
}

void ANPC_Spirit::SetDissolveAmount(float const DissolveAmount)
{
	for (TObjectPtr<UStaticMeshComponent> MeshComponent : StaticMeshComponents)
	{
		int32 MaterialCount = MeshComponent->GetNumMaterials();
		for (int32 i = 0; i < MaterialCount; i++)
		{
			TObjectPtr<UMaterialInterface> Material = MeshComponent->GetMaterial(i);
			if (Material)
			{
				// Create a dynamic material instance
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
				if (DynamicMaterial)
				{
					// Set the dissolve amount
					DynamicMaterial->SetScalarParameterValue(FName("FadeToBlackValue"), DissolveAmount);

					// Apply the dynamic material instance to the mesh component
					MeshComponent->SetMaterial(i, DynamicMaterial);
				}
			}
		}
	}

	if (DissolveAmount >= 1.0f)
	{
		this->Destroy();
	}
}


