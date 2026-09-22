// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "NPCs/NPCSpirit.h"
#include "NPCs/NPCFactory.h"
#include "Player/PlayerCharacter.h"
#include "NPCs/NPCDissolveLatentAction.h"
#include "Player/DifficultyAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "GameLogic/HUDWidget.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "NavigationInvokerComponent.h"

// Sets default values
ANPCSpirit::ANPCSpirit()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HandCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HandCollision"));
	if (HandCollision)
	{
		HandCollision->SetupAttachment(GetMesh(), TEXT("Right_Hand_Socket"));
		HandCollision->OnComponentBeginOverlap.AddDynamic(this, &ANPCSpirit::OnAttackOverlapBegin);
		HandCollision->OnComponentEndOverlap.AddDynamic(this, &ANPCSpirit::OnAttackOverlapEnd);
	}

	AbilitySystemComponent = CreateDefaultSubobject<UDifficultyAbilitySystemComponent>("AbilitySystemComponent");
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	}

	NavigationInvoker = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvoker"));
}

// Called when the game starts or when spawned
void ANPCSpirit::BeginPlay()
{
	Super::BeginPlay();

	// Apply property-driven component setup here so Blueprint default overrides are respected.
	if (HandCollision)
	{
		HandCollision->SetRelativeLocation(HandCollisionOffset);
	}
	if (NavigationInvoker)
	{
		NavigationInvoker->SetGenerationRadii(SightRadius * 2.0f, SightRadius * 2.5f);
	}

	PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (MaterialParameterCollection)
	{
		MPCInstance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	}

	if (MPCInstance)
	{
		MPCInstance->SetScalarParameterValue(FName("DissolveAmount"), 0.0f);
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
	
	NPCFactoryRef = Cast<ANPCFactory>(UGameplayStatics::GetActorOfClass(GetWorld(), ANPCFactory::StaticClass()));
}

void ANPCSpirit::Destroyed()
{
	Super::Destroyed();
	if (!NPCFactoryRef)
	{
		if (UWorld* World = GetWorld())
		{
			NPCFactoryRef = Cast<ANPCFactory>(UGameplayStatics::GetActorOfClass(World, ANPCFactory::StaticClass()));
		}
	}
	
	if (NPCFactoryRef)
	{
		NPCFactoryRef->ResetSpawnState();
	}
}

void ANPCSpirit::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, const int32 OtherBodyIndex, const bool bFromSweep, FHitResult const& SweepResult)
{
	if (OtherActor != this && bIsNPCInAttackMode)
	{
		if (APlayerCharacter* const PCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			const float NewHealth = FMath::Max(PCharacter->GetCurrentMentalHealth() - AttackValue, 0.0f);
			PCharacter->SetMentalHealth(NewHealth);
		}
	}
}

void ANPCSpirit::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent, AActor* const OtherActor, UPrimitiveComponent* const OtherComponent, const int32 OtherBodyIndex)
{
	// Only the player leaving the hand collider should refresh the mental health widget.
	if (!Cast<APlayerCharacter>(OtherActor))
	{
		return;
	}

	if (!WidgetController)
	{
		// Get all UI widgets
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UHUDWidget::StaticClass(), false);

		if (FoundWidgets.Num() > 0)
		{
			WidgetController = Cast<UHUDWidget>(FoundWidgets[0]);
		}
	}

	if (WidgetController)
	{
		WidgetController->ResetVisibility(EWidgetType::MentalHealth);
	}
}

// Called every frame
void ANPCSpirit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerActor)
	{
		PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

	if (bIsRotating)
	{
		if (!PlayerActor)
		{
			bIsRotating = false;
			return;
		}

		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerActor->GetActorLocation());
		const FRotator InterRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, RotationSpeed);
		SetActorRotation(InterRotation);
		if (InterRotation.Equals(LookAtRotation, 1.0f))
		{
			bIsRotating = false;
		}
	}
}

UAbilitySystemComponent* ANPCSpirit::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ANPCSpirit::SetDefaultAbilities()
{
	if (AbilitySystemComponent)
	{
		for (auto AbilityClass : DefaultAbilities)
		{
			if (AbilityClass)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, INDEX_NONE, this));
			}
		}
	}
}

UBehaviorTree* ANPCSpirit::GetBehaviorTree()
{
	return BehaviorTree;
}

UAnimMontage* ANPCSpirit::GetAnimationMontage() const
{
	return AnimationMontage;
}

void ANPCSpirit::Attack()
{
	// Rotate towards player
	if (PlayerActor)
	{
		bIsRotating = true;
	}

	if (AnimationMontage)
	{
		PlayAnimMontage(AnimationMontage);
	}
}

void ANPCSpirit::AttackStart()
{
	bIsNPCInAttackMode = true;
	if (HandCollision)
	{
		// Profile name must match DefaultEngine.ini ([/Script/Engine.CollisionProfile]).
		HandCollision->SetCollisionProfileName("SpiritHandPreset");
		HandCollision->SetNotifyRigidBodyCollision(true);
	}
}

void ANPCSpirit::AttackEnd()
{
	if (HandCollision)
	{
		HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HandCollision->SetNotifyRigidBodyCollision(false);
	}
	bIsNPCInAttackMode = false;
	bHasNPCAttacked = true;
}

void ANPCSpirit::StartDissolve()
{
	if (!bIsNPCDissolving)
	{
		bIsNPCDissolving = true;
		// Monotonic ID avoids colliding with other latent actions on this actor.
		static int32 NextDissolveLatentID = 0;
		const int32 DissolveID = NextDissolveLatentID++;
		GetWorld()->GetLatentActionManager().AddNewAction(this, DissolveID,
        		new NPCDissolveLatentAction(DissolveID, DissolveDuration, this));
	}
	
}

void ANPCSpirit::SetDissolveAmount(float const DissolveAmount)
{
	if (MPCInstance)
	{
		MPCInstance->SetScalarParameterValue(FName("DissolveAmount"), DissolveAmount);
	}

	if (DissolveAmount >= 1.0f)
	{
		this->Destroy();
	}
}



