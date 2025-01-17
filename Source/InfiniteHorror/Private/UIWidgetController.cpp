// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "UIWidgetController.h"
#include "Components/Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

float currentVisibility = 0.0f;
APlayerCharacter* PlayerCharacter;

void UUIWidgetController::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	AllWidgets.Empty();
	CollectAllWidgets(AllWidgets);

	if (AllWidgets.Num() > 0)
	{
		for (UWidget* Widget : AllWidgets)
		{
			if (Widget)
			{
				Widget->SetVisibility(InitialVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			}
		}
	}

	currentVisibility = VisibilityCooldown;
}

void UUIWidgetController::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	currentVisibility -= InDeltaTime;

	for (UWidget* Widget : AllWidgets)
	{
		if (Widget)
		{
			Widget->SetRenderOpacity(FMath::Max(currentVisibility / VisibilityCooldown, 0.0f));
		}
	}
}

void UUIWidgetController::CollectAllWidgets(TArray<UWidget*>& OutWidgets)
{
	if (WidgetTree)
	{
		WidgetTree->GetAllWidgets(OutWidgets);
	}
}

float UUIWidgetController::CalculateMentalHealthPercentage()
{
	if (PlayerCharacter)
	{
		return UKismetMathLibrary::NormalizeToRange(PlayerCharacter->GetCurrentMentalHealth(), 0.0f, 100.0f);
	}
	return 0.0f;
}

float UUIWidgetController::UpdateBatteryPercentage()
{
	if (PlayerCharacter)
	{
		return UKismetMathLibrary::NormalizeToRange(PlayerCharacter->GetCurrentBatteryLevel(), 0.0f, 100.0f);
	}
	return 0.0f;
}
