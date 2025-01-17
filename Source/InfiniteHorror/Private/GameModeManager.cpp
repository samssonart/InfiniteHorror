// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "GameModeManager.h"

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
