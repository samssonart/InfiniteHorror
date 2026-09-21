// Copyright (c) 2024 - 2026 Samssonart. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InfiniteHorrorTarget : TargetRules
{
	public InfiniteHorrorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("InfiniteHorror");
	}
}
