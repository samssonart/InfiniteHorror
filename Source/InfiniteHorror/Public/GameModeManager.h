// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameSettings.h"
#include "UIWidgetController.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeManager.generated.h"

/**
 * @brief Custom implementation of the game mode manager.
 */
UCLASS()
class INFINITEHORROR_API AGameModeManager : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	/**
	 * @brief Called when the game starts or when the actor is spawned.
	 */
	virtual void BeginPlay() override;

public:

	/**
	 * @brief Constructor for the AGameModeManager class.
	 */
	AGameModeManager(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * @brief Blueprint class reference for the UI widget controller.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI") 
	TSubclassOf<UUIWidgetController> UIWidgetBP;

	/**
	 * @brief Pointer to the GameSettings instance.
	 */
	TObjectPtr<UGameSettings> GameSettings;
};
