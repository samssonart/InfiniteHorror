// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/SLoadingScreen.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

void SLoadingScreen::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
		.BorderBackgroundColor(FLinearColor::Black)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(64.0f)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("LoadingScreen", "Loading", "LOADING..."))
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 24))
			.ColorAndOpacity(FLinearColor::White)
		]
	];
}
