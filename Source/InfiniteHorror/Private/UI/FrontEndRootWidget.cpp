// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/FrontEndRootWidget.h"
#include "CommonActivatableWidget.h"
#include "InfiniteHorrorLog.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

void UFrontEndRootWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (bFlowStarted)
	{
		return;
	}
	bFlowStarted = true;

	SplashQueue->OnDisplayedWidgetChanged().AddUObject(this, &UFrontEndRootWidget::HandleSplashDisplayChanged);

	bool bAnySplashQueued = false;
	for (const TSubclassOf<UCommonActivatableWidget>& SplashClass : SplashScreens)
	{
		bAnySplashQueued |= SplashQueue->AddWidget<UCommonActivatableWidget>(SplashClass) != nullptr;
	}

	if (!bAnySplashQueued)
	{
		ShowMainMenu();
	}
}

void UFrontEndRootWidget::HandleSplashDisplayChanged(UCommonActivatableWidget* DisplayedWidget)
{
	// The queue displays nothing once its last screen has finished.
	if (!DisplayedWidget)
	{
		ShowMainMenu();
	}
}

void UFrontEndRootWidget::ShowMainMenu()
{
	if (bMainMenuShown)
	{
		return;
	}
	bMainMenuShown = true;

	UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Showing the main menu."), *GetName());
	if (!MainMenuClass || !MenuStack->AddWidget<UCommonActivatableWidget>(MainMenuClass))
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("%s: MainMenuClass is not set or could not be created."), *GetName());
	}
}
