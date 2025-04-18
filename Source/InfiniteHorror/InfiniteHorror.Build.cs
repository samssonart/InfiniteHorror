// Copyright (c) 2024 - 2025 Samssonart. All rights reserved.

using UnrealBuildTool;

public class InfiniteHorror : ModuleRules
{
	public InfiniteHorror(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", 
			"Foliage", "AIModule", "NavigationSystem", "GameplayTasks", "Slate", "SlateCore", "UMG", "GameplayAbilities", "GameplayTasks", "GameplayTags" });

		//PrivateDependencyModuleNames.AddRange(new string[] { "Foliage" });

		// Uncomment if you are using Slate UI
		 PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
