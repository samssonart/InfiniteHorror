// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/ButtonMenuWidget.h"
#include "UI/MenuButton.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBoxSlot.h"
#include "InfiniteHorrorLog.h"

void UButtonMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	RebuildButtons();
}

void UButtonMenuWidget::AddDefaultEntry(const FText& Label, FName Callback)
{
	FMenuEntry& Entry = Entries.AddDefaulted_GetRef();
	Entry.Label = Label;
	Entry.Callback = Callback;
}

void UButtonMenuWidget::RebuildButtons()
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
			UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: ButtonClass is not set; the menu will have no buttons."), *GetName());
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
		Button->OnClicked().AddUObject(this, &UButtonMenuWidget::HandleEntryClicked, EntryIndex);

		if (UVerticalBoxSlot* ButtonSlot = Cast<UVerticalBoxSlot>(ButtonContainer->AddChild(Button)))
		{
			ButtonSlot->SetPadding(ButtonPadding);
			ButtonSlot->SetHorizontalAlignment(HAlign_Fill);
		}
		GeneratedButtons.Add(Button);
	}
}

UWidget* UButtonMenuWidget::NativeGetDesiredFocusTarget() const
{
	return GeneratedButtons.Num() > 0 ? GeneratedButtons[0].Get() : Super::NativeGetDesiredFocusTarget();
}

void UButtonMenuWidget::HandleEntryClicked(int32 EntryIndex)
{
	if (!Entries.IsValidIndex(EntryIndex))
	{
		return;
	}

	const FName Callback = Entries[EntryIndex].Callback;
	UFunction* Function = FindFunction(Callback);
	if (!Function || Function->NumParms != 0)
	{
		UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: menu entry %d callback '%s' is not a parameterless function on this widget."), *GetName(), EntryIndex, *Callback.ToString());
		return;
	}

	ProcessEvent(Function, nullptr);
}

TArray<FName> UButtonMenuWidget::GetCallbackOptions() const
{
	TArray<FName> Options;
	for (TFieldIterator<UFunction> It(GetClass(), EFieldIteratorFlags::IncludeSuper); It; ++It)
	{
		const UFunction* Function = *It;
		if (Function->NumParms != 0 || Function->HasAnyFunctionFlags(FUNC_Delegate))
		{
			continue;
		}

		// Only offer functions introduced by the menu or its Blueprints, not engine
		// functions or Blueprint overrides of engine events such as Construct.
		const UFunction* Original = Function;
		while (const UFunction* SuperFunction = Original->GetSuperFunction())
		{
			Original = SuperFunction;
		}
		if (Original->GetOwnerClass()->IsChildOf(UButtonMenuWidget::StaticClass()))
		{
			Options.AddUnique(Function->GetFName());
		}
	}
	Options.Sort(FNameLexicalLess());
	return Options;
}
