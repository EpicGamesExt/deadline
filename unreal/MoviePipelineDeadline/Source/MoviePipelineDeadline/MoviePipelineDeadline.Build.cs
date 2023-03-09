// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MoviePipelineDeadline : ModuleRules
{
	public MoviePipelineDeadline(ReadOnlyTargetRules Target) : base(Target)
	{
		ShortName = "DMP";

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"InputCore",
				"RenderCore",
				"Engine",
				"MovieRenderPipelineCore",
				"DeveloperSettings",
				"DeadlineService"
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
            }
        );
	}
}
