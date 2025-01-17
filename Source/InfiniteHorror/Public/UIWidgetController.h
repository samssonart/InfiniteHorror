// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UIWidgetController.generated.h"

/**
 * Wrapper for UI Widget to define custom behaviors
 */
UCLASS()
class INFINITEHORROR_API UUIWidgetController : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void CollectAllWidgets(TArray<UWidget*>& OutWidgets);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* BatteryBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MentalHealthBar;

	UFUNCTION(BlueprintPure)
	float CalculateMentalHealthPercentage();

	UFUNCTION(BlueprintPure)
	float UpdateBatteryPercentage();



private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility", meta = (AllowPrivateAccess = "true"))
	float VisibilityCooldown = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visibility", meta = (AllowPrivateAccess = "true"))
	bool InitialVisibility = true;

	TArray<UWidget*> AllWidgets;
};
