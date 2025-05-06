// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "DifficultyAbilitySystemComponent.generated.h"

UENUM(BlueprintType)
enum class EDifficulty : uint8
{
	None UMETA(DisplayName = "None"),
	Easy UMETA(DisplayName = "Easy"),
	Medium UMETA(DisplayName = "Medium"),
	Hard UMETA(DisplayName = "Hard"),
};

UCLASS()
class INFINITEHORROR_API UDifficultyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UDifficultyAbilitySystemComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
};
