// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "FrontEndRootWidget.generated.h"

class UCommonActivatableWidget;
class UCommonActivatableWidgetQueue;
class UCommonActivatableWidgetStack;

/**
 * @brief Root of the front-end UI. Plays the splash screens one after another, then shows the main menu.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API UFrontEndRootWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void HandleSplashDisplayChanged(UCommonActivatableWidget* DisplayedWidget);
	void ShowMainMenu();

	/** Screens shown at startup, in order. */
	UPROPERTY(EditDefaultsOnly, Category = "Front End")
	TArray<TSubclassOf<UCommonActivatableWidget>> SplashScreens;

	/** Screen shown once the splash screens have finished. */
	UPROPERTY(EditDefaultsOnly, Category = "Front End")
	TSubclassOf<UCommonActivatableWidget> MainMenuClass;

	/* Container that plays SplashScreens; bound by name from the UMG designer. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetQueue> SplashQueue;

	/* Container for the main menu and any screens it opens; bound by name from the UMG designer. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;

	bool bFlowStarted = false;
	bool bMainMenuShown = false;
};
