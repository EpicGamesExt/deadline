// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlineJobDataAsset.h"
#include "JsonObjectConverter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DeadlineJobDataAsset)

DEFINE_LOG_CATEGORY(LogDeadlineDataAsset);
DEFINE_LOG_CATEGORY(LogDeadlineStruct);


/**
 * Returns the Deadline job info preset struct as a Json String
 */
FString UDeadlineJobPresetLibrary::GetJobInfoPresetJsonString(FString PresetName)
{
	if (this->DeadlineJobPresets.Contains(PresetName))
	{
		FString JobOutputString;
		if (FJsonObjectConverter::UStructToJsonObjectString(
				this->DeadlineJobPresets[PresetName].JobInfoPreset,
				JobOutputString
				))
		{
			/*
			* This Json Object returns keys as camelCasing instead of PascalCasing as Deadline will expect. A
			* sanitation system will need to be setup to convert from camel casing to pascal casing
			*/
			return JobOutputString;
		}
		else
		{
			UE_LOG(LogDeadlineStruct, Error, TEXT("UDeadlineJobInfoStruct - Unable to convert properties to Json string."));
			return "";
		}
	}
	else
	{
		UE_LOG(LogDeadlineDataAsset, Error, TEXT("UDeadlineJobPresetLibrary - The preset does not exists in the library."));
		return "";
	}

}

/**
 * Returns the Deadline plugin info preset struct as a Json String
 */
FString UDeadlineJobPresetLibrary::GetPluginInfoPresetJsonString(FString PresetName)
{
	if (this->DeadlineJobPresets.Contains(PresetName))
	{
		FString JobOutputString;
		if (FJsonObjectConverter::UStructToJsonObjectString(
				this->DeadlineJobPresets[PresetName].PluginInfoPreset,
				JobOutputString
				))
		{
			/*
			* This Json Object returns keys as camelCasing instead of PascalCasing as Deadline will expect. A
			* sanitation system will need to be setup to convert from camel casing to pascal casing
			*/
			return JobOutputString;
		}
		else
		{
			UE_LOG(LogDeadlineStruct, Error, TEXT("UDeadlinePluginInfoStruct - Unable to convert properties to Json string."));
			return "";
		}
		
	}
	else
	{
		UE_LOG(LogDeadlineDataAsset, Error, TEXT("UDeadlineJobPresetLibrary - The preset does not exists in the library."));
		return "";
	}
}
