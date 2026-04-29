// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MasterProject21 : ModuleRules
{
	public MasterProject21(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
