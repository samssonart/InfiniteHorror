// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InfiniteHorrorEditorTarget : TargetRules
{
	public InfiniteHorrorEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("InfiniteHorror");
	}
}
