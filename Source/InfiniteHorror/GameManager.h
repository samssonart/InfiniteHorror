// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"

/**
 * @brief Game instance for game-wide systems and state.
 */
UCLASS()
class INFINITEHORROR_API UGameManager : public UGameInstance
{
	GENERATED_BODY()

public:
	/**
	 * @brief Opens a map with a loading screen covering the blocking load. The loading screen only
	 * appears outside the editor because the movie player is disabled in PIE.
	 * @param Map The map to open.
	 * @return False if Map is not set, in which case no travel happens.
	 */
	UFUNCTION(BlueprintCallable, Category = "Travel")
	bool OpenMapWithLoadingScreen(const TSoftObjectPtr<UWorld>& Map);

protected:
	/**
	 * @brief Loads and applies the user's settings once at startup. Must not happen during a map
	 * load: applying settings can resize the viewport, which crashes the renderer while the movie
	 * player's loading screen is drawing on its own thread.
	 */
	virtual void OnStart() override;
};

