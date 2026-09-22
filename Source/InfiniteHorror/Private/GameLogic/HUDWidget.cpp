// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "GameLogic/HUDWidget.h"
#include "InfiniteHorrorLog.h"
#include "Components/Widget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> AllWidgets;
	CollectAllWidgets(AllWidgets);

	TrackedWidgets.Reset();
	VisibilityTimers.Reset();
	BatteryWidget = nullptr;
	MentalHealthWidget = nullptr;

	// GetAllWidgets returns the root widget at index 0; it is not tracked.
	for (int32 i = 1; i < AllWidgets.Num(); ++i)
	{
		UWidget* Widget = AllWidgets[i];
		if (!Widget)
		{
			continue;
		}

		Widget->SetVisibility(InitialVisibility ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		TrackedWidgets.Add(Widget);
		VisibilityTimers.Add(VisibilityCooldown);

		if (Widget->GetName().Contains(TEXT("Battery")))
		{
			BatteryWidget = Widget;
		}
		else if (Widget->GetName().Contains(TEXT("MentalHealth")))
		{
			MentalHealthWidget = Widget;
		}
	}

	if (!BatteryWidget)
	{
		UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: no widget with 'Battery' in its name found; torch HUD will not refresh."), *GetName());
	}
	if (!MentalHealthWidget)
	{
		UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: no widget with 'MentalHealth' in its name found; mental health HUD will not refresh."), *GetName());
	}
}

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	for (int32 i = 0; i < VisibilityTimers.Num(); ++i)
	{
		VisibilityTimers[i] = FMath::Max(VisibilityTimers[i] - InDeltaTime, 0.0f);
		if (UWidget* Widget = TrackedWidgets[i])
		{
			Widget->SetRenderOpacity(VisibilityTimers[i] / VisibilityCooldown);
		}
	}
}

void UHUDWidget::CollectAllWidgets(TArray<UWidget*>& OutWidgets)
{
	if (WidgetTree)
	{
		WidgetTree->GetAllWidgets(OutWidgets);
	}
}

APlayerCharacter* UHUDWidget::GetPlayerCharacter()
{
	if (!PlayerCharacter)
	{
		PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	return PlayerCharacter;
}

float UHUDWidget::CalculateMentalHealthPercentage()
{
	if (APlayerCharacter* Character = GetPlayerCharacter())
	{
		return UKismetMathLibrary::NormalizeToRange(Character->GetCurrentMentalHealth(), 0.0f, 100.0f);
	}
	return 0.0f;
}

float UHUDWidget::UpdateBatteryPercentage()
{
	if (APlayerCharacter* Character = GetPlayerCharacter())
	{
		return UKismetMathLibrary::NormalizeToRange(Character->GetCurrentBatteryLevel(), 0.0f, 100.0f);
	}
	return 0.0f;
}

void UHUDWidget::ResetVisibility(EWidgetType WidgetType)
{
	UWidget* TargetWidget = nullptr;
	switch (WidgetType)
	{
		case EWidgetType::Battery:
			TargetWidget = BatteryWidget;
			break;
		case EWidgetType::MentalHealth:
			TargetWidget = MentalHealthWidget;
			break;
		default:
			break;
	}

	const int32 Index = TrackedWidgets.IndexOfByKey(TargetWidget);
	if (Index != INDEX_NONE)
	{
		VisibilityTimers[Index] = VisibilityCooldown;
	}
}


