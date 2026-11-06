// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/MainMenuWidget.h"
#include "InfiniteHorror/GameManager.h"
#include "CommonInputModeTypes.h"
#include "Input/UIActionBindingHandle.h"
#include "InfiniteHorrorLog.h"
#include "Kismet/KismetSystemLibrary.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, GameplayMap(FSoftObjectPath(TEXT("/Game/Levels/GameplayMap.GameplayMap")))
{
	AddDefaultEntry(NSLOCTEXT("MainMenu", "StartGame", "START GAME"), GET_FUNCTION_NAME_CHECKED(UMainMenuWidget, StartGame));
	AddDefaultEntry(NSLOCTEXT("MainMenu", "Settings", "SETTINGS"), GET_FUNCTION_NAME_CHECKED(UMainMenuWidget, OpenSettings));
	AddDefaultEntry(NSLOCTEXT("MainMenu", "Gallery", "GALLERY"), GET_FUNCTION_NAME_CHECKED(UMainMenuWidget, OpenGallery));
	AddDefaultEntry(NSLOCTEXT("MainMenu", "Quit", "QUIT"), GET_FUNCTION_NAME_CHECKED(UMainMenuWidget, QuitToDesktop));
}

TOptional<FUIInputConfig> UMainMenuWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture);
}

void UMainMenuWidget::StartGame()
{
	if (bStartRequested)
	{
		return;
	}

	UGameManager* GameManager = GetGameInstance<UGameManager>();
	if (!GameManager)
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("%s: The game instance is not a UGameManager; cannot start the game."), *GetName());
		return;
	}

	if (GameManager->OpenMapWithLoadingScreen(GameplayMap))
	{
		bStartRequested = true;
		SetIsEnabled(false);
	}
}

void UMainMenuWidget::OpenSettings_Implementation()
{
	// TODO: Push the settings screen once it exists.
	UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Settings requested; settings screen not implemented yet."), *GetName());
}

void UMainMenuWidget::OpenGallery_Implementation()
{
	// TODO: Push the gallery screen once it exists.
	UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Gallery requested; gallery screen not implemented yet."), *GetName());
}

void UMainMenuWidget::QuitToDesktop()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
