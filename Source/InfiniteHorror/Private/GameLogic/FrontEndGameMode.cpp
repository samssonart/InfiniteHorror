// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "GameLogic/FrontEndGameMode.h"
#include "UI/FrontEndRootWidget.h"
#include "InfiniteHorrorLog.h"
#include "GameFramework/PlayerController.h"

AFrontEndGameMode::AFrontEndGameMode()
{
	DefaultPawnClass = nullptr;
}

void AFrontEndGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (HasActorBegunPlay())
	{
		ShowRootWidget();
	}
}

void AFrontEndGameMode::BeginPlay()
{
	Super::BeginPlay();
	ShowRootWidget();
}

void AFrontEndGameMode::ShowRootWidget()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (RootWidget || !PlayerController)
	{
		return;
	}

	if (!RootWidgetClass)
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("%s: RootWidgetClass is not set; the front end cannot start."), *GetName());
		return;
	}

	RootWidget = CreateWidget<UFrontEndRootWidget>(PlayerController, RootWidgetClass);
	if (RootWidget)
	{
		RootWidget->AddToViewport();
	}
}
