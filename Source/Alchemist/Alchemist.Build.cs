// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Alchemist : ModuleRules
{
	public Alchemist(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "UniversalRpgElements", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameplayTasks", "AIModule", "NavigationSystem"});
	}
}
