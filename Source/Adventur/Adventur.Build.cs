// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Adventur : ModuleRules
{
	public Adventur(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "GameplayTasks", "Slate", "SlateCore" });
	}
}
