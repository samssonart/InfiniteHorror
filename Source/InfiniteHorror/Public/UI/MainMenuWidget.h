// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/ButtonMenuWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * @brief The game's main menu, shown in the front-end map once the splash screens have finished.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UMainMenuWidget : public UButtonMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	/**
	 * @brief Menu-only input with a free, visible mouse cursor.
	 */
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	/**
	 * @brief Travels to GameplayMap. Further clicks are ignored once travel has started.
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void StartGame();

	/**
	 * @brief Called when the Settings option is chosen. Override in Blueprint until the settings screen exists.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Main Menu")
	void OpenSettings();

	/**
	 * @brief Called when the Gallery option is chosen. Override in Blueprint until the gallery screen exists.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Main Menu")
	void OpenGallery();

	/**
	 * @brief Exits to desktop.
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void QuitToDesktop();

private:
	/** Map opened by Start Game. A soft reference so the front end doesn't load gameplay content. */
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu")
	TSoftObjectPtr<UWorld> GameplayMap;

	bool bStartRequested = false;
};
