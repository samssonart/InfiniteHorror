// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"

/**
 * @brief Singleton manager for game-wide systems and state.
 */
UCLASS()
class INFINITEHORROR_API UGameManager : public UGameInstance
{
    GENERATED_BODY()

public:
    /**
     * @brief Retrieves the singleton instance of the GameManager.
     * @return The singleton instance.
     */
    static TObjectPtr<UGameManager> GetInstance();

    /**
     * @brief Initializes the game manager.
     */
    void Init();

private:
    /** Singleton instance of the GameManager. */
    static TObjectPtr<UGameManager> Instance;
};
