// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/PauseMenuWidget.h"
#include "UI/MenuButton.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "CommonInputModeTypes.h"
#include "Input/UIActionBindingHandle.h"
#include "InfiniteHorrorLog.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// The back action (if configured in CommonUI settings) behaves like Resume.
	bIsBackHandler = true;

	FPauseMenuEntry& ResumeEntry = Entries.AddDefaulted_GetRef();
	ResumeEntry.Label = NSLOCTEXT("PauseMenu", "Resume", "RESUME");
	ResumeEntry.Callback = GET_FUNCTION_NAME_CHECKED(UPauseMenuWidget, ResumeGame);

	FPauseMenuEntry& SettingsEntry = Entries.AddDefaulted_GetRef();
	SettingsEntry.Label = NSLOCTEXT("PauseMenu", "Settings", "SETTINGS");
	SettingsEntry.Callback = GET_FUNCTION_NAME_CHECKED(UPauseMenuWidget, OpenSettings);
}

TOptional<FUIInputConfig> UPauseMenuWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::NoCapture);
}

void UPauseMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	RebuildButtons();
}

void UPauseMenuWidget::RebuildButtons()
{
	if (!ButtonContainer)
	{
		return;
	}

	ButtonContainer->ClearChildren();
	GeneratedButtons.Reset();

	if (!ButtonClass)
	{
		if (!IsDesignTime())
		{
			UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: ButtonClass is not set; the pause menu will have no buttons."), *GetName());
		}
		return;
	}

	for (int32 EntryIndex = 0; EntryIndex < Entries.Num(); ++EntryIndex)
	{
		UMenuButton* Button = CreateWidget<UMenuButton>(this, ButtonClass);
		if (!Button)
		{
			continue;
		}

		Button->SetButtonText(Entries[EntryIndex].Label);
		Button->OnClicked().AddUObject(this, &UPauseMenuWidget::HandleEntryClicked, EntryIndex);

		if (UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(ButtonContainer->AddChild(Button)))
		{
			ButtonSlot->SetPadding(ButtonPadding);
			ButtonSlot->SetHorizontalAlignment(HAlign_Fill);
		}
		GeneratedButtons.Add(Button);
	}
}

UWidget* UPauseMenuWidget::NativeGetDesiredFocusTarget() const
{
	return GeneratedButtons.Num() > 0 ? GeneratedButtons[0].Get() : Super::NativeGetDesiredFocusTarget();
}

void UPauseMenuWidget::HandleEntryClicked(int32 EntryIndex)
{
	if (!Entries.IsValidIndex(EntryIndex))
	{
		return;
	}

	const FName Callback = Entries[EntryIndex].Callback;
	UFunction* Function = FindFunction(Callback);
	if (!Function || Function->NumParms != 0)
	{
		UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: pause menu entry %d callback '%s' is not a parameterless function on this widget."), *GetName(), EntryIndex, *Callback.ToString());
		return;
	}

	ProcessEvent(Function, nullptr);
}

TArray<FName> UPauseMenuWidget::GetCallbackOptions() const
{
	TArray<FName> Options;
	for (TFieldIterator<UFunction> It(GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		const UFunction* Function = *It;
		if (Function->NumParms != 0 || Function->HasAnyFunctionFlags(FUNC_Delegate))
		{
			continue;
		}

		// Only offer functions introduced by the pause menu or its Blueprints, not engine
		// functions or Blueprint overrides of engine events such as Construct.
		const UFunction* Original = Function;
		while (const UFunction* SuperFunction = Original->GetSuperFunction())
		{
			Original = SuperFunction;
		}
		if (Original->GetOwnerClass()->IsChildOf(UPauseMenuWidget::StaticClass()))
		{
			Options.AddUnique(Function->GetFName());
		}
	}
	Options.Sort(FNameLexicalLess());
	return Options;
}

void UPauseMenuWidget::ResumeGame()
{
	DeactivateWidget();
}

void UPauseMenuWidget::OpenSettings_Implementation()
{
	// TODO: Push the settings screen once it exists.
	UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Settings requested; settings screen not implemented yet."), *GetName());
}
