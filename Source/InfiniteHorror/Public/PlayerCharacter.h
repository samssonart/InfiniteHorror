// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "UIWidgetController.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerCharacter.generated.h"

class UDifficultyAbilitySystemComponent;

/**
 * The player character class
 */
UCLASS()
class INFINITEHORROR_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	explicit APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	* @brief Returns the current mental health of the player
	* @return The current mental health of the player
	*/
	float GetCurrentMentalHealth() const { return MentalHealth; }

	/*
	* @brief Returns the current battery level of the torch
	* @return The current battery level of the torch
	*/
	float GetCurrentBatteryLevel() const { return BatteryLevel; }

	/*
	* @bried Sets the mental health of the player
	* @param NewHealth The value to set mental health to
	*/
	void SetMentalHealth(float const NewHealth) { MentalHealth = NewHealth; }


protected:

	/*
	 *@brief Sets the default abilities of the player
	 */
	void SetDefaultAbilities();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:	
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimuli();
	void ToggleTorch();

	TObjectPtr<UUIWidgetController> WidgetController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDifficultyAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bIsTorchOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float MentalHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float BatteryLevel = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float BatteryDepletionRate = 0.15f;

};
