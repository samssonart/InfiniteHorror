// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "Player/PlayerCharacterState.h"
#include "Player/DifficultyAbilitySystemComponent.h"
#include "Player/PlayerAttributeSet.h"

UAbilitySystemComponent* APlayerCharacterState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPlayerAttributeSet* APlayerCharacterState::GetAttributeSet() const
{
	return PlayerAttributeSet;
}

// Sets default values
APlayerCharacterState::APlayerCharacterState()
{
	PrimaryActorTick.bCanEverTick = false;
	AbilitySystemComponent = CreateDefaultSubobject<UDifficultyAbilitySystemComponent>("AbilitySystemComponent");
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	}

	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>("PlayerAttributeSet");
}



