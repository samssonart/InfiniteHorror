// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

class UDifficultyAbilitySystemComponent;
class UPlayerAttributeSet;

UCLASS()
class INFINITEHORROR_API APlayerCharacterState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerCharacterState();

	/*
	* @brief Returns the ability system component of the player
	*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/*
	* @brief Returns the attribute set of the player
	*/
	virtual UPlayerAttributeSet* GetAttributeSet() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDifficultyAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;
};

