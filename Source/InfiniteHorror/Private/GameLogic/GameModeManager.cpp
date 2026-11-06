// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "GameLogic/GameModeManager.h"
#include "GameLogic/GameSettings.h"
#include "GameLogic/HUDWidget.h"
#include "UI/SLoadingScreen.h"
#include "World/PatchStreamingManager.h"
#include "InfiniteHorrorLog.h"
#include "Engine/GameViewportClient.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

namespace
{
	/** Above every UMG widget the game adds to the viewport, including the HUD. */
	constexpr int32 LoadingOverlayZOrder = 10000;
}

void AGameModeManager::BeginPlay()
{
	Super::BeginPlay();

	// Added here so it is already up when the movie player's loading screen ends.
	APatchStreamingManager* PatchManager = FindPatchStreamingManager();
	if (PatchManager && !PatchManager->AreInitialPatchesLoaded())
	{
		ShowLoadingOverlay();
		PatchManager->OnInitialPatchesLoaded.AddUniqueDynamic(this, &AGameModeManager::OnInitialPatchesLoaded);
	}

	if (GEngine)
	{
		GameSettings = Cast<UGameSettings>(GEngine->GetGameUserSettings());
	}

	if (GameSettings)
	{
		// Set the game difficulty to a random value. This is just for testing purposes
		// TODO: Remove this code and set the difficulty based on user input
		EDifficulty RandomDifficulty = StaticCast<EDifficulty>(FMath::RandRange(1, 3));
		FString InfoMessage = FString::Printf(TEXT("Difficulty set to %d"), StaticCast<int>(RandomDifficulty));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InfoMessage);
		GameSettings->SetGameDifficulty(RandomDifficulty);
	}
	else
	{
		UE_LOG(LogInfiniteHorror, Error, TEXT("GameUserSettings is not a UGameSettings. Set GameUserSettingsClassName=/Script/InfiniteHorror.GameSettings under [/Script/Engine.Engine] in DefaultEngine.ini."));
	}

	if (UIWidgetBP)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), UIWidgetBP);
		if (Widget)
		{
			Widget->AddToViewport(UIWidgetZOrder);
		}
	}
}

void AGameModeManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	HideLoadingOverlay();
	Super::EndPlay(EndPlayReason);
}

void AGameModeManager::ShowLoadingOverlay()
{
	UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
	if (GameViewport && !LoadingOverlay.IsValid())
	{
		LoadingOverlay = SNew(SLoadingScreen);
		GameViewport->AddViewportWidgetContent(LoadingOverlay.ToSharedRef(), LoadingOverlayZOrder);
		UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Loading overlay shown while the initial terrain loads."), *GetName());
	}
}

void AGameModeManager::HideLoadingOverlay()
{
	if (LoadingOverlay.IsValid())
	{
		if (UGameViewportClient* GameViewport = GetWorld()->GetGameViewport())
		{
			GameViewport->RemoveViewportWidgetContent(LoadingOverlay.ToSharedRef());
		}
		LoadingOverlay.Reset();
		UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Loading overlay removed."), *GetName());
	}
}

void AGameModeManager::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	APatchStreamingManager* PatchManager = FindPatchStreamingManager();
	if (!PatchManager || PatchManager->AreInitialPatchesLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
		return;
	}

	if (const AActor* StartSpot = FindPlayerStart(NewPlayer))
	{
		PatchManager->RequestInitialLoad(StartSpot->GetActorLocation());
	}

	PlayersWaitingForTerrain.AddUnique(NewPlayer);
	PatchManager->OnInitialPatchesLoaded.AddUniqueDynamic(this, &AGameModeManager::OnInitialPatchesLoaded);
}

void AGameModeManager::OnInitialPatchesLoaded()
{
	TArray<TObjectPtr<APlayerController>> Players = MoveTemp(PlayersWaitingForTerrain);
	PlayersWaitingForTerrain.Reset();

	for (APlayerController* Player : Players)
	{
		if (IsValid(Player))
		{
			Super::HandleStartingNewPlayer_Implementation(Player);
		}
	}

	HideLoadingOverlay();
}

APatchStreamingManager* AGameModeManager::FindPatchStreamingManager() const
{
	return Cast<APatchStreamingManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APatchStreamingManager::StaticClass()));
}



