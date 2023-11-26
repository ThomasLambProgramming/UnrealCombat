// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UniversalRpgElements : ModuleRules
{
	public UniversalRpgElements(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateDependencyModuleNames.AddRange(new string[] {  "Core", "CoreUObject", "Engine", "GameplayTasks", });
	}
}
