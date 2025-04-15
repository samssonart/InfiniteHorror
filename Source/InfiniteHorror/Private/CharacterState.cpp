// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "CharacterState.h"
#include "DifficultyAbilitySystemComponent.h"

UAbilitySystemComponent* ACharacterState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Sets default values
ACharacterState::ACharacterState()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterState::BeginPlay()
{
	Super::BeginPlay();
	
}


