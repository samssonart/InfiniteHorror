// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/PauseMenuWidget.h"
#include "CommonInputModeTypes.h"
#include "Input/UIActionBindingHandle.h"
#include "InfiniteHorrorLog.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// The back action (if configured in CommonUI settings) behaves like Resume.
	bIsBackHandler = true;

	AddDefaultEntry(NSLOCTEXT("PauseMenu", "Resume", "RESUME"), GET_FUNCTION_NAME_CHECKED(UPauseMenuWidget, ResumeGame));
	AddDefaultEntry(NSLOCTEXT("PauseMenu", "Settings", "SETTINGS"), GET_FUNCTION_NAME_CHECKED(UPauseMenuWidget, OpenSettings));
}

TOptional<FUIInputConfig> UPauseMenuWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);
}

void UPauseMenuWidget::ResumeGame()
{
	DeactivateWidget();
}

void UPauseMenuWidget::OpenSettings_Implementation()
{
	// TODO: Push the settings screen once it exists.
	UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Settings requested; settings screen not implemented yet."), *GetName());
}
