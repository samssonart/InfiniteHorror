// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DifficultyAbilitySystemComponent.h"
#include "GameSettings.generated.h"

/**
 * @brief Class is used to store the game settings
 */
UCLASS()
class INFINITEHORROR_API UGameSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:

	/*
	 *@brief Constructor
	 */
	UGameSettings(const FObjectInitializer& ObjectInitializer);
	
	/*
	* @brief Sets the game difficulty
	* @param NewValue The new difficulty value, as an enum
	*/
	UFUNCTION(BlueprintCallable)
	void SetGameDifficulty(Difficulty NewValue);

	/*
	* @brief Gets the current game difficulty
	*/
	UFUNCTION(BlueprintCallable, Category = "Settings")
	Difficulty GetCurrentDifficulty() const;

	/*
	* @brief Gets settings from file or default values
	*/
	void InitializeSettings();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	Difficulty GameDifficulty;

	static UGameUserSettings* GetGameUserSettings();
};
