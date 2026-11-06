// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * @brief Full-screen loading screen. Pure Slate because the movie player renders it off the game
 * thread during a map load. The gameplay map reuses it until its terrain is ready, so the two
 * loading phases look like one.
 */
class INFINITEHORROR_API SLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingScreen) {}
	SLATE_END_ARGS()

	/**
	 * @brief Builds the widget.
	 * @param InArgs Slate construction arguments.
	 */
	void Construct(const FArguments& InArgs);
};
