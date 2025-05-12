// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealTesting : ModuleRules
{
	public UnrealTesting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "AIModule", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
