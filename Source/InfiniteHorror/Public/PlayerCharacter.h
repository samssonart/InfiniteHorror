// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "ScalableFloat.h"
#include "PlayerAttributeSet.h"
#include "GameFramework/Character.h"
#include "UIWidgetController.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerCharacter.generated.h"

class UDifficultyAbilitySystemComponent;

/**
 *@brief The player character class
 */
UCLASS()
class INFINITEHORROR_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the APlayerCharacter class.
	 */
	explicit APlayerCharacter();
	
	/**
	 * @brief Called every frame to update the character.
	 * @param DeltaTime The time elapsed since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * @brief Called when the character is possessed by a controller.
	 * @param NewController The controller that possesses the character.
	 */
	virtual void PossessedBy(AController* NewController) override;
	
	/**
	 * @brief Retrieves the ability system component associated with the character.
	 * @return A pointer to the ability system component.
	 */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	/**
	 * @brief Retrieves the attribute set associated with the character.
	 * @return A pointer to the player attribute set.
	 */
	virtual UPlayerAttributeSet* GetAttributeSet() const;

	/**
	* @brief Returns the current mental health of the player
	* @return The current mental health of the player
	*/
	float GetCurrentMentalHealth() const { return MentalHealth; }

	/**
	* @brief Returns the current battery level of the torch
	* @return The current battery level of the torch
	*/
	float GetCurrentBatteryLevel() const { return BatteryLevel; }

	/**
	* @brief Sets the mental health of the player
	* @param NewHealth The value to set mental health to
	*/
	void SetMentalHealth(float const NewHealth) { MentalHealth = NewHealth; }


protected:

	/**
	 * @brief The maximum mental health value for the player, scalable based on difficulty.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Difficulty")
	FScalableFloat MentalHealthMax;
	
	/**
	 * @brief The movement speed of the player, scalable based on difficulty.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Difficulty")
	FScalableFloat MoveSpeed;
	
	/**
	 * @brief The endurance level of the player, scalable based on difficulty.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Difficulty")
	FScalableFloat Endurance;
	
	/**
	* @brief Sets up the ability system for the player
	*/
	void InitAbilitySystemComponent();
	
	/**
	 *@brief Sets the default abilities of the player
	 */
	void SetDefaultAbilities();
	
	/**
	* @brief Sets the difficulty-dependent values of the player
	*/
	void SetDifficultyAttributes() const;
	
	/**
	 * @brief Called when the game starts or when the actor is spawned.
	 */
	virtual void BeginPlay() override;
	
	/**
	 * @brief Sets up player input bindings for the character.
	 * @param PlayerInputComponent The input component to bind inputs to.
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:	
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimuli();
	void ToggleTorch();

	TObjectPtr<UUIWidgetController> WidgetController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDifficultyAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DifficultyEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float MentalHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float BatteryLevel = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	float BatteryDepletionRate = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", meta = (AllowPrivateAccess = "true"))
	bool bIsTorchOn = false;

};
