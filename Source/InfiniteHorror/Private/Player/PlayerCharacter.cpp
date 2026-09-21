// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterState.h"
#include "Player/PlayerAttributeSet.h"
#include "Player/DifficultyAbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameLogic/GameModeManager.h"
#include "GameLogic/GameSettings.h"
#include "GameLogic/HUDWidget.h"
#include "InfiniteHorrorLog.h"
#include "Kismet/GameplayStatics.h"
#include "InputAction.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


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

	// Find the Input Action asset as a default; can be overridden per Blueprint.
	static ConstructorHelpers::FObjectFinder<UInputAction> TorchActionFinder(TEXT("/Game/Input/IA_Torch"));
	if (TorchActionFinder.Succeeded())
	{
		TorchAction = TorchActionFinder.Object;
	}
}

void APlayerCharacter::InitAbilitySystemComponent()
{
	APlayerCharacterState* PlayerStateRef = GetPlayerState<APlayerCharacterState>();
	if (!PlayerStateRef)
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("%s: PlayerState is not APlayerCharacterState; ability system disabled. Check PlayerStateClass in the game mode."), *GetName());
		return;
	}

	AbilitySystemComponent = Cast<UDifficultyAbilitySystemComponent>(PlayerStateRef->GetAbilitySystemComponent());
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("%s: PlayerState has no UDifficultyAbilitySystemComponent; ability system disabled."), *GetName());
		return;
	}

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

		const TObjectPtr<AGameModeManager> GameModeManager =
			Cast<AGameModeManager>(UGameplayStatics::GetGameMode(GetWorld()));

		if (SpecHandle.IsValid() && GameModeManager && GameModeManager->GameSettings)
		{
			int GameDifficultyNumeric = StaticCast<int>(GameModeManager->GameSettings->GetCurrentDifficulty());
			float MentalHealthCurrent = MentalHealthMax.GetValueAtLevel(GameDifficultyNumeric);
			float EnduranceCurrent = Endurance.GetValueAtLevel(GameDifficultyNumeric);
			float StaminaCurrent = MoveSpeed.GetValueAtLevel(GameDifficultyNumeric);
			FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
			FGameplayTag CallerTag = FGameplayTag::RequestGameplayTag(FName("Data.MentalHealth.Level"));
			Spec->SetSetByCallerMagnitude(CallerTag, MentalHealthCurrent);
			CallerTag = FGameplayTag::RequestGameplayTag(FName("Data.Endurance.Level"));
			Spec->SetSetByCallerMagnitude(CallerTag, EnduranceCurrent);
			CallerTag = FGameplayTag::RequestGameplayTag(FName("Data.Stamina.Level"));
			Spec->SetSetByCallerMagnitude(CallerTag, StaminaCurrent);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec);
		}
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// The widget may not exist yet; FindWidgetController is retried lazily in Tick.
	FindWidgetController();
}

void APlayerCharacter::FindWidgetController()
{
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UHUDWidget::StaticClass(), false);

	if (FoundWidgets.Num() > 0)
	{
		// Assuming you have only one instance or you want the first one found
		WidgetController = Cast<UHUDWidget>(FoundWidgets[0]);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsTorchOn)
	{
		if (!WidgetController)
		{
			FindWidgetController();
		}
		if (WidgetController)
		{
			WidgetController->ResetVisibility(EWidgetType::Battery);
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
		if (TorchAction)
		{
			EnhancedInputComponent->BindAction(TorchAction, ETriggerEvent::Completed, this, &APlayerCharacter::ToggleTorch);
		}
		else
		{
			UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: TorchAction is not set; torch input will not be bound."), *GetName());
		}
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


