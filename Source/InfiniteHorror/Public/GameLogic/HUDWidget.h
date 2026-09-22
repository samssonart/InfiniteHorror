// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
 * @brief  HUD widget that manages visibility and updates of its child widgets
 */
UCLASS()
class INFINITEHORROR_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void CollectAllWidgets(TArray<UWidget*>& OutWidgets);

	UFUNCTION(BlueprintCallable, Category = "Visibility")
	void ResetVisibility(EWidgetType WidgetType);

protected:

	UFUNCTION(BlueprintPure)
	float CalculateMentalHealthPercentage();

	UFUNCTION(BlueprintPure)
	float UpdateBatteryPercentage();



private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility", meta = (AllowPrivateAccess = "true"))
	float VisibilityCooldown = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility", meta = (AllowPrivateAccess = "true"))
	bool InitialVisibility = true;

	APlayerCharacter* GetPlayerCharacter();

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

