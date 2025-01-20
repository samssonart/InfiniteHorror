// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.


#include "UIWidgetController.h"
#include "Components/Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Engine/Engine.h"

APlayerCharacter* PlayerCharacter;

void UUIWidgetController::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	AllWidgets.Empty();
	CurrentVisibilityIndices.Empty();
	CollectAllWidgets(AllWidgets);

	if (AllWidgets.Num() > 0)
	{
		CurrentVisibilityIndices.Init(VisibilityCooldown, AllWidgets.Num() - 1);
		for (int i = 0; i < AllWidgets.Num(); i++)
		{
			UWidget* Widget = AllWidgets[i];
			if (Widget)
			{
				Widget->SetVisibility(InitialVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
				if (Widget->GetName().Contains("Battery"))
				{
					FString InfoMessage = FString::Printf(TEXT("Battery Bar found on index %d"), i);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InfoMessage);
					BatteryWidgetIndex = i;
				}
				else if (Widget->GetName().Contains("MentalHealth"))
				{
					FString InfoMessage = FString::Printf(TEXT("MentalHealth Bar found on index %d"), i);
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, InfoMessage);
					MentalHealthWidgetIndex = i;
				}
			}
		}
	}
}

void UUIWidgetController::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int v = 0; v < CurrentVisibilityIndices.Num(); v++)
	{
		CurrentVisibilityIndices[v] -= InDeltaTime;
		UWidget* Widget = AllWidgets[v+1];
		if (Widget)
		{
			Widget->SetRenderOpacity(FMath::Max(CurrentVisibilityIndices[v] / VisibilityCooldown, 0.0f));
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

void UUIWidgetController::ResetVisibility(int WidgetIndex)
{
	CurrentVisibilityIndices[WidgetIndex] = VisibilityCooldown;
}
