// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SplashScreenWidget.generated.h"

/**
 * @brief One full-screen startup screen. Deactivates itself after DisplayDuration seconds, or as
 * soon as any key or button is pressed. Its container then moves on to the next screen.
 */
UCLASS(Abstract, Blueprintable)
class INFINITEHORROR_API USplashScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Menu-only input with the mouse captured so the cursor stays hidden.
	 */
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	/**
	 * @brief Ends this screen immediately.
	 */
	UFUNCTION(BlueprintCallable, Category = "Splash Screen")
	void Skip();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

private:
	/** Seconds the screen stays up when nothing is pressed. */
	UPROPERTY(EditDefaultsOnly, Category = "Splash Screen", meta = (ClampMin = "0.1"))
	float DisplayDuration = 5.0f;

	/* Listens for the skip input while this widget is the active one. */
	TSharedPtr<class IInputProcessor> SkipInputProcessor;

	FTimerHandle DisplayTimerHandle;
};
