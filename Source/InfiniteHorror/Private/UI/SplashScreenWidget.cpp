// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.


#include "UI/SplashScreenWidget.h"
#include "CommonInputModeTypes.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/IInputProcessor.h"
#include "GameFramework/InputSettings.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/UIActionBindingHandle.h"
#include "InfiniteHorrorLog.h"
#include "TimerManager.h"

namespace
{
	/** Stick deflections arrive as key events too, and a drifting stick must not skip screens. */
	bool IsStickDirectionKey(const FKey& Key)
	{
		return Key == EKeys::Gamepad_LeftStick_Up || Key == EKeys::Gamepad_LeftStick_Down
			|| Key == EKeys::Gamepad_LeftStick_Left || Key == EKeys::Gamepad_LeftStick_Right
			|| Key == EKeys::Gamepad_RightStick_Up || Key == EKeys::Gamepad_RightStick_Down
			|| Key == EKeys::Gamepad_RightStick_Left || Key == EKeys::Gamepad_RightStick_Right;
	}

	/**
	 * Skips a splash screen on any fresh key, gamepad button or mouse button press. Runs ahead of widget
	 * focus so nothing has to be focused. Input is observed but never consumed, which keeps
	 * editor commands such as Esc to stop PIE working.
	 */
	class FSplashSkipInputProcessor : public IInputProcessor
	{
	public:
		explicit FSplashSkipInputProcessor(USplashScreenWidget& InSplashScreen)
			: SplashScreen(&InSplashScreen)
		{
		}

		virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
		{
		}

		virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
		{
			const FKey Key = InKeyEvent.GetKey();

			// Holding a key must not race through every screen, and modifiers cover shortcuts such as Alt+Tab.
			const bool bIgnored = InKeyEvent.IsRepeat()
				|| Key.IsModifierKey()
				|| InKeyEvent.IsAltDown()
				|| InKeyEvent.IsCommandDown()
				|| IsStickDirectionKey(Key)
				|| GetDefault<UInputSettings>()->ConsoleKeys.Contains(Key);

			if (!bIgnored)
			{
				RequestSkip();
			}
			return false;
		}

		virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
		{
			RequestSkip();
			return false;
		}

		virtual const TCHAR* GetDebugName() const override
		{
			return TEXT("SplashSkipInputProcessor");
		}

	private:
		void RequestSkip() const
		{
			if (USplashScreenWidget* Widget = SplashScreen.Get())
			{
				Widget->Skip();
			}
		}

		TWeakObjectPtr<USplashScreenWidget> SplashScreen;
	};
}

TOptional<FUIInputConfig> USplashScreenWidget::GetDesiredInputConfig() const
{
	return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::CapturePermanently, true);
}

void USplashScreenWidget::Skip()
{
	if (IsActivated())
	{
		DeactivateWidget();
	}
}

void USplashScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkipInputProcessor.IsValid())
	{
		return;
	}

	// CommonUI registers a linked processor only while this widget is receiving input and removes it afterwards.
	UCommonUIActionRouterBase* ActionRouter = ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(GetOwningLocalPlayer());
	SkipInputProcessor = MakeShared<FSplashSkipInputProcessor>(*this);
	if (!ActionRouter || !ActionRouter->RegisterLinkedPreprocessor(*this, SkipInputProcessor.ToSharedRef()))
	{
		UE_LOG(LogInfiniteHorror, Warning, TEXT("%s: Could not register the skip input; this screen can only end on its timer."), *GetName());
	}
}

void USplashScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	UE_LOG(LogInfiniteHorror, Log, TEXT("%s: Splash screen shown."), *GetName());
	GetWorld()->GetTimerManager().SetTimer(DisplayTimerHandle, this, &USplashScreenWidget::Skip, DisplayDuration, false);
}

void USplashScreenWidget::NativeOnDeactivated()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DisplayTimerHandle);
	}

	Super::NativeOnDeactivated();
}
