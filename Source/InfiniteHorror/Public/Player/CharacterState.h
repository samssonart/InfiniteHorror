// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "CharacterState.generated.h"

class UDifficultyAbilitySystemComponent;
class UPlayerAttributeSet;

UCLASS()
class INFINITEHORROR_API ACharacterState : public APlayerState, public IAbilitySystemInterface
{
public:

	/*
	* @brief Returns the ability system component of the player
	*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/*
	* @brief Returns the attribute set of the player
	*/
	virtual UPlayerAttributeSet* GetAttributeSet() const;

	// Sets default values for this actor's properties
	ACharacterState();

private:
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDifficultyAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
