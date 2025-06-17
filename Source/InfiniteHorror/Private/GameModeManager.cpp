// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "GameModeManager.h"
#include "GameSettings.h"
#include "UIWidgetController.h"

AGameModeManager::AGameModeManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    if(GEngine)
    {
        TObjectPtr<UGameUserSettings> UserSettings = GEngine->GetGameUserSettings();
        	if (UserSettings)
        	{
        		GameSettings = Cast<UGameSettings>(UserSettings);
        	}
    }
	
	if (GameSettings)
	{
		GameSettings->InitializeSettings();
		// Set the game difficulty to a random value. This is just for testing purposes
		// TODO: Remove this code and set the difficulty based on user input
		EDifficulty randomDifficulty = StaticCast<EDifficulty>(FMath::RandRange(1, 3));
		FString InfoMessage = FString::Printf(TEXT("Difficulty set to %d"), StaticCast<int>(randomDifficulty));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InfoMessage);
		GameSettings->SetGameDifficulty(randomDifficulty);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameSettings not found!"));
	}
}

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
}


