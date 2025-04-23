// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "PlayerCharacter.h"
#include "CharacterState.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DifficultyAbilitySystemComponent.h"
#include "InputAction.h"

UInputAction* IA_Torch;

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPlayerAttributeSet* APlayerCharacter::GetAttributeSet() const
{
	return PlayerAttributeSet;
}

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetupStimuli();

	// Find the Input Action asset
	static ConstructorHelpers::FObjectFinder<UInputAction> TorchAction(TEXT("/Game/Input/IA_Torch"));
	if (TorchAction.Succeeded())
	{
		IA_Torch = TorchAction.Object;
	}
}

void APlayerCharacter::InitAbilitySystemComponent()
{
	ACharacterState* PlayerStateRef = GetPlayerState<ACharacterState>();
	check(PlayerStateRef);
	AbilitySystemComponent = CastChecked<UDifficultyAbilitySystemComponent>(PlayerStateRef->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(PlayerStateRef, this);
	PlayerAttributeSet = PlayerStateRef->GetAttributeSet();
}

void APlayerCharacter::SetDefaultAbilities()
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

void APlayerCharacter::SetDifficultyAttributes() const
{
	if (AbilitySystemComponent && DifficultyEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle =
			AbilitySystemComponent->MakeOutgoingSpec(DifficultyEffect, 1.0f, EffectContext);

		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUIWidgetController::StaticClass(), false);

	if (FoundWidgets.Num() > 0)
	{
		// Assuming you have only one instance or you want the first one found
		WidgetController = Cast<UUIWidgetController>(FoundWidgets[0]);
	}
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsTorchOn)
	{
		if (WidgetController)
		{
			WidgetController->ResetVisibility(EWidgetType::Torch);
		}
		BatteryLevel -= DeltaTime * BatteryDepletionRate;
		if (BatteryLevel <= 0.0f)
		{
			bIsTorchOn = false;
			BatteryLevel = 0.0f;
			return;
		}
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystemComponent();
	SetDifficultyAttributes();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Torch, ETriggerEvent::Completed, this, &APlayerCharacter::ToggleTorch);
	}
}

void APlayerCharacter::SetupStimuli()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus Source"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSource->RegisterWithPerceptionSystem();
	}
}

void APlayerCharacter::ToggleTorch()
{
	if (BatteryLevel > 0)
	{
		bIsTorchOn = !bIsTorchOn;
	}
	else
	{
		bIsTorchOn = false;
	}
}

