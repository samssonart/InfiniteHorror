// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "GameLogic/GameSettings.h"
#include "InfiniteHorrorLog.h"

UGameSettings::UGameSettings(const FObjectInitializer& ObjectInitializer)
{
	SetToDefaults();
}

void UGameSettings::SetGameDifficulty(EDifficulty NewValue)
{
	GameDifficulty = NewValue;
}

EDifficulty UGameSettings::GetCurrentDifficulty() const
{
	return GameDifficulty;
}

void UGameSettings::InitializeSettings()
{
	// Try to load from file
	LoadSettings();
	
	if (GameDifficulty == EDifficulty::None)
	{
		UE_LOG(LogInfiniteHorror, Warning, TEXT("GameSettings difficulty not set!"));
		// If not set, use default value
		GameDifficulty = EDifficulty::Medium;
	}

	// Save the settings to file
	ApplySettings(false);
}


