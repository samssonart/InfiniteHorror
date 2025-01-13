// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InfiniteHorrorEditorTarget : TargetRules
{
	public InfiniteHorrorEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("InfiniteHorror");
	}
}
