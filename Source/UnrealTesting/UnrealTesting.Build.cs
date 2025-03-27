// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealTesting : ModuleRules
{
	public UnrealTesting(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
