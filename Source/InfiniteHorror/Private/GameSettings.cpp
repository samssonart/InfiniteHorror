// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "GameSettings.h"

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
}

UGameUserSettings* UGameSettings::GetGameUserSettings()
{
	static ConstructorHelpers::FClassFinder<UGameSettings> SettingsClass(TEXT("/Script/InfiniteHorror.GameSettings"));
	return NewObject<UGameSettings>(SettingsClass.Class);
}

