// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "GameModeManager.h"
#include "GameSettings.h"

void AGameModeManager::BeginPlay()
{
	Super::BeginPlay();
	if (UIWidgetBP)
	{
		UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), UIWidgetBP);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}

	UGameSettings* GameSettings = Cast<UGameSettings>(UGameUserSettings::GetGameUserSettings());
	if (GameSettings)
	{
		GameSettings->InitializeSettings();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSettings not found!"));
	}
}
