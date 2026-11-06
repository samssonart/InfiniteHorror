// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FrontEndGameMode.generated.h"

class UFrontEndRootWidget;

/**
 * @brief Game mode for the front-end map: no pawn or HUD, only the startup UI.
 */
UCLASS()
class INFINITEHORROR_API AFrontEndGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFrontEndGameMode();

	/**
	 * @brief Shows the front-end UI if play has already begun. Spawns no pawn.
	 * @param NewPlayer The player controller that is ready to enter the game.
	 */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:
	/**
	 * @brief Shows the front-end UI if the player has already joined.
	 */
	virtual void BeginPlay() override;

private:
	/* A map load logs the player in before BeginPlay while PIE does so after, hence the two callers. */
	void ShowRootWidget();

	/** Root widget that runs the splash screens and main menu. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UFrontEndRootWidget> RootWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UFrontEndRootWidget> RootWidget;
};
