// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

// GameModeManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeManager.generated.h"

class UGameSettings;
class UUIWidgetController;

/**
 * @brief Custom implementation of the game mode manager.
 */
UCLASS()
class INFINITEHORROR_API AGameModeManager : public AGameModeBase
{
    GENERATED_BODY()

public:
    /**
     * @brief Constructor for the AGameModeManager class.
     * @param ObjectInitializer The object initializer.
     */
    AGameModeManager(const FObjectInitializer& ObjectInitializer);

protected:
    /**
     * @brief Called when the game starts or when the actor is spawned.
     */
    virtual void BeginPlay() override;

public:
    /**
     * @brief Blueprint class reference for the UI widget controller.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUIWidgetController> UIWidgetBP;

    /**
     * @brief Pointer to the GameSettings instance.
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings")
    TObjectPtr<UGameSettings> GameSettings;
};