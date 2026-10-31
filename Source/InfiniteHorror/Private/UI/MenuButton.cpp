// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/MenuButton.h"
#include "CommonTextBlock.h"

void UMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Runs in the designer too, so label edits are previewed immediately.
	SetButtonText(ButtonText);
	ApplyCurrentTextStyle();
}

void UMenuButton::NativeOnCurrentTextStyleChanged()
{
	Super::NativeOnCurrentTextStyleChanged();
	ApplyCurrentTextStyle();
}

void UMenuButton::ApplyCurrentTextStyle()
{
	// CommonUI only notifies on text style changes; applying it to the label is up to the button.
	if (LabelText)
	{
		if (const TSubclassOf<UCommonTextStyle> TextStyle = GetCurrentTextStyleClass())
		{
			LabelText->SetStyle(TextStyle);
		}
	}
}

void UMenuButton::SetButtonText(const FText& NewText)
{
	ButtonText = NewText;
	if (LabelText)
	{
		LabelText->SetText(ButtonText);
	}
}
