// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

class UInputAction;
class UPauseMenuWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseMenuStateChanged, bool, bIsOpen);

/**
 * @brief Player controller for the main character. Owns the pause menu and the game's pause state.
 */
UCLASS()
class INFINITEHORROR_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the AMainCharacterController class.
	 */
	explicit AMainCharacterController();

	/**
	 * @brief Pauses the game and shows the pause menu.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OpenPauseMenu();

	/**
	 * @brief Closes the pause menu and resumes the game.
	 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClosePauseMenu();

	/**
	 * @brief Returns whether the pause menu is currently shown.
	 * @return True if the pause menu is active.
	 */
	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsPauseMenuOpen() const;

	/** Broadcast after the pause menu opens (true) or closes (false). */
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnPauseMenuStateChanged OnPauseMenuStateChanged;

protected:
	/**
	 * @brief Binds the controller-level input actions.
	 */
	virtual void SetupInputComponent() override;

private:
	void TogglePauseMenu();

	/* Runs however the menu was closed (Resume button, back action, or the pause key). */
	void HandlePauseMenuDeactivated();

	/** Toggles the pause menu. Must have bTriggerWhenPaused enabled so it can also close the menu. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPauseMenuWidget> PauseMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	int32 PauseMenuZOrder = 100;

	/* Created on first open and reused afterwards. */
	UPROPERTY(Transient)
	TObjectPtr<UPauseMenuWidget> PauseMenuWidget = nullptr;
};
