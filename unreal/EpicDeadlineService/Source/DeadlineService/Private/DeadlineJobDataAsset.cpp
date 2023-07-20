// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlineJobDataAsset.h"

#include "DeadlineServiceEditorSettings.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DeadlineJobDataAsset)

DEFINE_LOG_CATEGORY(LogDeadlineDataAsset);
DEFINE_LOG_CATEGORY(LogDeadlineStruct);


UDeadlineJobPreset::UDeadlineJobPreset()
{
	// Set default values good for most users
	FString ExecutablePath = FString(FPlatformProcess::ExecutablePath());
	ExecutablePath.RemoveFromEnd(".exe");
	ExecutablePath += "-Cmd.exe";
	
	JobPresetStruct.PluginInfo.Add("Executable", ExecutablePath);
	JobPresetStruct.PluginInfo.Add("ProjectFile", FPaths::GetProjectFilePath());
	JobPresetStruct.PluginInfo.Add("CommandLineArguments","-log");
}
