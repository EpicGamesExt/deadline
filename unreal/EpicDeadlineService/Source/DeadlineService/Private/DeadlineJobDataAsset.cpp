// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlineJobDataAsset.h"

#include "DeadlineServiceEditorSettings.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DeadlineJobDataAsset)

DEFINE_LOG_CATEGORY(LogDeadlineDataAsset);
DEFINE_LOG_CATEGORY(LogDeadlineStruct);


UDeadlineJobPresetLibrary::UDeadlineJobPresetLibrary()
{
	// Set default values good for most users
	FString ExecutablePath = FString(FPlatformProcess::ExecutablePath());
	ExecutablePath.RemoveFromEnd(".exe");
	ExecutablePath += "-Cmd.exe";
	
	JobInfo.PluginInfoPreset.Add("Executable", ExecutablePath);
	JobInfo.PluginInfoPreset.Add("ProjectFile", FPaths::GetProjectFilePath());
	JobInfo.PluginInfoPreset.Add("CommandLineArguments","-log");
}
