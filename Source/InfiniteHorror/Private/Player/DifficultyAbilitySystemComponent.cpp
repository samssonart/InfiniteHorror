// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "Player/DifficultyAbilitySystemComponent.h"

// Sets default values
UDifficultyAbilitySystemComponent::UDifficultyAbilitySystemComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicated(true);
}

// Called when the game starts or when spawned
void UDifficultyAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
}



