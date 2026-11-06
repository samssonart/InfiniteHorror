// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "Player/MainCharacterController.h"
#include "UI/PauseMenuWidget.h"
#include "InfiniteHorrorLog.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "CommonInputModeTypes.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/UIActionBindingHandle.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/LocalPlayer.h"

AMainCharacterController::AMainCharacterController()
{
	// Find the Input Action asset as a default; can be overridden per Blueprint.
	static ConstructorHelpers::FObjectFinder<UInputAction> PauseMenuActionFinder(TEXT("/Game/Input/IA_PauseMenu"));
	if (PauseMenuActionFinder.Succeeded())
	{
		PauseMenuAction = PauseMenuActionFinder.Object;
	}
}

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();
	ApplyGameInputConfig();
}

void AMainCharacterController::ApplyGameInputConfig()
{
	if (UCommonUIActionRouterBase* ActionRouter = ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(GetLocalPlayer()))
	{
		ActionRouter->SetActiveUIInputConfig(FUIInputConfig(ECommonInputMode::Game, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown));
	}
}

void AMainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseMenuAction)
		{
			EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Started, this, &AMainCharacterController::TogglePauseMenu);
		}
		else
		{
			UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: PauseMenuAction is not set; pause menu input will not be bound."), *GetName());
		}
	}
}

void AMainCharacterController::TogglePauseMenu()
{
	if (IsPauseMenuOpen())
	{
		ClosePauseMenu();
	}
	else if (GetPawn())
	{
		// No pawn means the terrain is still loading. The menu would be hidden behind the loading
		// overlay, and pausing would stop the timers that end the load.
		OpenPauseMenu();
	}
}

bool AMainCharacterController::IsPauseMenuOpen() const
{
	return PauseMenuWidget && PauseMenuWidget->IsActivated();
}

void AMainCharacterController::OpenPauseMenu()
{
	if (IsPauseMenuOpen())
	{
		return;
	}

	if (!PauseMenuWidget)
	{
		if (!PauseMenuClass)
		{
			UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: PauseMenuClass is not set; cannot open the pause menu."), *GetName());
			return;
		}

		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuClass);
		if (!PauseMenuWidget)
		{
			return;
		}
		PauseMenuWidget->OnDeactivated().AddUObject(this, &AMainCharacterController::HandlePauseMenuDeactivated);
	}

	if (!PauseMenuWidget->IsInViewport())
	{
		PauseMenuWidget->AddToViewport(PauseMenuZOrder);
	}

	SetPause(true);
	// Activation hands focus and the menu's input config to CommonUI.
	PauseMenuWidget->ActivateWidget();
	OnPauseMenuStateChanged.Broadcast(true);
}

void AMainCharacterController::ClosePauseMenu()
{
	if (IsPauseMenuOpen())
	{
		PauseMenuWidget->DeactivateWidget();
	}
}

void AMainCharacterController::HandlePauseMenuDeactivated()
{
	PauseMenuWidget->RemoveFromParent();
	SetPause(false);
	ApplyGameInputConfig();

	OnPauseMenuStateChanged.Broadcast(false);
}
