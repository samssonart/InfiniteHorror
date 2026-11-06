// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#include "GameManager.h"
#include "GameLogic/GameSettings.h"
#include "UI/SLoadingScreen.h"
#include "InfiniteHorrorLog.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "MoviePlayer.h"

void UGameManager::OnStart()
{
	Super::OnStart();

	if (UGameSettings* GameSettings = Cast<UGameSettings>(GEngine->GetGameUserSettings()))
	{
		GameSettings->InitializeSettings();
	}
}

bool UGameManager::OpenMapWithLoadingScreen(const TSoftObjectPtr<UWorld>& Map)
{
	if (Map.IsNull())
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("%s: Cannot open a map that is not set."), *GetName());
		return false;
	}

	// The movie player starts this by itself when the map load begins and ends it when the load
	// completes. Don't wait for manual stop here: that would block the game thread after the load
	// without ticking the world, so anything the new map streams in would never finish.
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreen.bMoviesAreSkippable = false;
	LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen);
	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Map);
	return true;
}
