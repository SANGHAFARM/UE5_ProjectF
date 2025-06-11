// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectF : ModuleRules
{
	public ProjectF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		InternalIncludePaths.Add("ProjectF");

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });
	}
}
