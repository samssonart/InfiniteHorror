// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "HUDWidget.generated.h"

class APlayerCharacter;

/**
 * @brief  Enum to define the different types of widgets
 */
UENUM(BlueprintType)
enum class EWidgetType : uint8
{
	Battery,
	MentalHealth
};


/**
 * @brief  HUD widget that manages visibility and updates of its child widgets.
 * Not activatable: it takes no input, and it draws above the pause menu, where an activatable
 * widget would take input routing away from the menu.
 */
UCLASS()
class INFINITEHORROR_API UHUDWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void CollectAllWidgets(TArray<UWidget*>& OutWidgets);

	UFUNCTION(BlueprintCallable, Category = "Visibility")
	void ResetVisibility(EWidgetType WidgetType);

protected:

	UFUNCTION(BlueprintPure)
	float CalculateMentalHealthPercentage();

	UFUNCTION(BlueprintPure)
	float UpdateBatteryPercentage();

	/**
	 * @brief Holds every tracked widget at full opacity while the pause menu is open;
	 * on close their timers restart from VisibilityCooldown so they fade out as usual.
	 * @param bIsOpen Whether the pause menu is now open.
	 */
	UFUNCTION()
	void HandlePauseMenuStateChanged(bool bIsOpen);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility", meta = (AllowPrivateAccess = "true"))
	float VisibilityCooldown = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility", meta = (AllowPrivateAccess = "true"))
	bool InitialVisibility = true;

	APlayerCharacter* GetPlayerCharacter();

	void ShowAllTrackedWidgets();

	/* True while the pause menu is open; tracked widgets do not fade. */
	bool bHoldFullyVisible = false;

	UPROPERTY(Transient)
	TObjectPtr<APlayerCharacter> PlayerCharacter = nullptr;

	/* Widgets whose opacity is driven by VisibilityTimers, parallel arrays. */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UWidget>> TrackedWidgets;
	TArray<float> VisibilityTimers;

	/* Resolved once in NativeConstruct by name; null if the widget was renamed in UMG. */
	UPROPERTY(Transient)
	TObjectPtr<UWidget> BatteryWidget;
	UPROPERTY(Transient)
	TObjectPtr<UWidget> MentalHealthWidget;
};

