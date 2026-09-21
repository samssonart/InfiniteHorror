// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

// GameModeManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeManager.generated.h"

class UGameSettings;
class UHUDWidget;
class APatchStreamingManager;

/**
 * @brief Custom implementation of the game mode manager.
 */
UCLASS()
class INFINITEHORROR_API AGameModeManager : public AGameModeBase
{
    GENERATED_BODY()

public:
    /**
     * @brief Defers pawn spawning until the initial terrain patches around the player start are loaded.
     * @param NewPlayer The player controller that is ready to enter the game.
     */
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

protected:
    /**
     * @brief Called when the game starts or when the actor is spawned.
     */
    virtual void BeginPlay() override;

    /**
     * @brief Spawns pawns for every player that was waiting on terrain streaming.
     */
    UFUNCTION()
    void OnInitialPatchesLoaded();

public:
    /**
     * @brief Blueprint class reference for the UI widget controller.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UHUDWidget> UIWidgetBP;

    /**
     * @brief Pointer to the GameSettings instance.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    TObjectPtr<UGameSettings> GameSettings;

private:
    APatchStreamingManager* FindPatchStreamingManager() const;

    UPROPERTY(Transient)
    TArray<TObjectPtr<APlayerController>> PlayersWaitingForTerrain;
};
