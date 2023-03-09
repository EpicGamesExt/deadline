// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeadlineService : ModuleRules
{
	public DeadlineService(ReadOnlyTargetRules Target) : base(Target)
	{

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine",
				"DeveloperSettings",
				"UnrealEd",
				"JsonUtilities"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
            }
        );
	}
}
