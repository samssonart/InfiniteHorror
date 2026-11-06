// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ButtonMenuWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * @brief In-game pause menu overlay. Pausing the game is owned by AMainCharacterController.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UPauseMenuWidget : public UButtonMenuWidget
{
	GENERATED_BODY()

public:
	UPauseMenuWidget(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief UI and game both receive input so the pause action can close the menu again;
	 * gameplay actions stay inert because the world is paused.
	 */
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	/**
	 * @brief Closes the menu, which resumes the game.
	 */
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ResumeGame();

	/**
	 * @brief Called when the Settings option is chosen. Override in Blueprint until the settings screen exists.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pause Menu")
	void OpenSettings();
};
