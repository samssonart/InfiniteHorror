// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "MenuButton.generated.h"

class UCommonTextBlock;

/**
 * @brief Generic CommonUI text button used by the in-game menus.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UMenuButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the label shown on the button.
	 * @param NewText The text to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(const FText& NewText);

protected:
	virtual void NativePreConstruct() override;

	/** Pushes the style's text style for the current state (normal/hovered/disabled) onto the label. */
	virtual void NativeOnCurrentTextStyleChanged() override;

private:
	void ApplyCurrentTextStyle();

	/** Label shown on the button; editable per instance in the parent widget's designer. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonText;

	/** Text block that displays ButtonText. Must be named "LabelText" in the UMG designer. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> LabelText;
};
