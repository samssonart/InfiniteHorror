// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ButtonMenuWidget.generated.h"

class UMenuButton;
class UPanelWidget;

/**
 * @brief One button in a menu: what it says and which function it calls.
 */
USTRUCT(BlueprintType)
struct FMenuEntry
{
	GENERATED_BODY()

	/** Text shown on the button. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu")
	FText Label;

	/** Parameterless function or custom event on the menu to call when the button is clicked. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menu", meta = (GetOptions = "GetCallbackOptions"))
	FName Callback;
};

/**
 * @brief Base for menus made of a vertical list of buttons. Builds one button per entry in
 * Entries and dispatches clicks to each entry's callback.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UButtonMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	/**
	 * @brief Adds a default entry. Intended for subclass constructors.
	 * @param Label Text shown on the button.
	 * @param Callback Name of a parameterless function on this widget.
	 */
	void AddDefaultEntry(const FText& Label, FName Callback);

private:
	/** Recreates the buttons in ButtonContainer from Entries. Runs in the designer too, for preview. */
	void RebuildButtons();

	void HandleEntryClicked(int32 EntryIndex);

	/** Editor dropdown source for FMenuEntry::Callback. */
	UFUNCTION()
	TArray<FName> GetCallbackOptions() const;

	/** Buttons shown in the menu, top to bottom. */
	UPROPERTY(EditDefaultsOnly, Category = "Menu", meta = (TitleProperty = "Label"))
	TArray<FMenuEntry> Entries;

	/** Widget class created for each entry. */
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<UMenuButton> ButtonClass;

	/** Spacing around each generated button. Applied when the container is a vertical box. */
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	FMargin ButtonPadding = FMargin(0.0f, 8.0f);

	/* Panel the generated buttons are added to; bound by name from the UMG designer. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ButtonContainer;

	/* Buttons created by RebuildButtons; the first receives focus when the menu opens. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UMenuButton>> GeneratedButtons;
};
