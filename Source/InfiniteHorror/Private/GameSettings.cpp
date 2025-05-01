// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "GameSettings.h"

UGameSettings::UGameSettings(const FObjectInitializer& ObjectInitializer)
{
	SetToDefaults();
}

void UGameSettings::SetGameDifficulty(Difficulty NewValue)
{
	GameDifficulty = NewValue;
}

Difficulty UGameSettings::GetCurrentDifficulty() const
{
	return GameDifficulty;
}

void UGameSettings::InitializeSettings()
{
	// Try to load from file
	LoadSettings();

	if (GameDifficulty == Difficulty::None)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSettings difficulty not set!"));
		// If not set, use default value
		GameDifficulty = Difficulty::Medium;
	}

	// Save the settings to file
	ApplySettings(false);
}

UGameUserSettings* UGameSettings::GetGameUserSettings()
{
	static ConstructorHelpers::FClassFinder<UGameSettings> SettingsClass(TEXT("/Script/InfiniteHorror.GameSettings"));
	return NewObject<UGameSettings>(SettingsClass.Class);
}

