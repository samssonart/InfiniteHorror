// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PauseMenuWidget.generated.h"

class UMenuButton;
class UPanelWidget;

/**
 * @brief One button in the pause menu: what it says and which function it calls.
 */
USTRUCT(BlueprintType)
struct FPauseMenuEntry
{
	GENERATED_BODY()

	/** Text shown on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause Menu")
	FText Label;

	/** Parameterless function or custom event on the pause menu to call when the button is clicked. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause Menu", meta = (GetOptions = "GetCallbackOptions"))
	FName Callback;
};

/**
 * @brief In-game pause menu overlay. Pausing the game is owned by AMainCharacterController;
 * this widget builds one button per entry in Entries and dispatches clicks to each entry's callback.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UPauseMenuWidget : public UCommonActivatableWidget
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

protected:
	virtual void NativePreConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

private:
	/** Recreates the buttons in ButtonContainer from Entries. Runs in the designer too, for preview. */
	void RebuildButtons();

	void HandleEntryClicked(int32 EntryIndex);

	/** Editor dropdown source for FPauseMenuEntry::Callback. */
	UFUNCTION()
	TArray<FName> GetCallbackOptions() const;

	/** Buttons shown in the menu, top to bottom. */
	UPROPERTY(EditDefaultsOnly, Category = "Pause Menu", meta = (TitleProperty = "Label"))
	TArray<FPauseMenuEntry> Entries;

	/** Widget class created for each entry. */
	UPROPERTY(EditDefaultsOnly, Category = "Pause Menu")
	TSubclassOf<UMenuButton> ButtonClass;

	/** Spacing around each generated button. Applied when the container is a vertical box. */
	UPROPERTY(EditDefaultsOnly, Category = "Pause Menu")
	FMargin ButtonPadding = FMargin(0.0f, 8.0f);

	/* Panel the generated buttons are added to; bound by name from the UMG designer. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ButtonContainer;

	/* Buttons created by RebuildButtons; the first receives focus when the menu opens. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UMenuButton>> GeneratedButtons;
};
