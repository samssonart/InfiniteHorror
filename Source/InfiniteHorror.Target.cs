// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class InfiniteHorrorTarget : TargetRules
{
	public InfiniteHorrorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("InfiniteHorror");
	}
}
