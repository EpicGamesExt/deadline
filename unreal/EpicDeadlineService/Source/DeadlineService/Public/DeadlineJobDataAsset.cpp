// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlineJobDataAsset.h"

#include "DeadlineServiceEditorSettings.h"

#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DeadlineJobDataAsset)

DEFINE_LOG_CATEGORY(LogDeadlineDataAsset);
DEFINE_LOG_CATEGORY(LogDeadlineStruct);


FDeadlineJobInfoStruct::FDeadlineJobInfoStruct()
{
	// Set default values good for most users
	FString ExecutablePath = FString(FPlatformProcess::ExecutablePath());
	ExecutablePath.RemoveFromEnd(".exe");
	ExecutablePath += "-Cmd.exe";
	
	PluginInfoPreset.Add("Executable", ExecutablePath);
	PluginInfoPreset.Add("ProjectFile", FPaths::GetProjectFilePath());
	PluginInfoPreset.Add("CommandLineArguments","-log");
}

/**
 * Returns the Deadline job info preset struct a TMap<FString,FString>
 */
TMap<FString, FString> UDeadlineJobPresetLibrary::GetJobInfoAsPythonCompatibleStringMap()
{
	TMap<FString, FString> ReturnValue = {{"Plugin", "UnrealEngine"}};
	FDeadlineJobInfoStruct* JobInfoToUse = &JobInfo;
	
	if (UDeadlineServiceEditorSettings* DeadlineServiceSettings = GetMutableDefault<UDeadlineServiceEditorSettings>())
	{
		ReturnValue["Plugin"] = DeadlineServiceSettings->PluginName;

		if (DeadlineServiceSettings->GetPresetOverrideSettings()->bShouldOverridePresetProperties)
		{
			JobInfoToUse = &DeadlineServiceSettings->GetPresetOverrideSettings()->PresetOverrides;
		}
	}
	
	for (TFieldIterator<FProperty> PropIt(FDeadlineJobInfoStruct::StaticStruct()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		if (!Property)
		{
			continue;
		}
		
		FName PropertyName = Property->GetFName();
		
		// Custom Handlers for specific properties prioritizing UX
		if (PropertyName.IsEqual("bSubmitJobAsSuspended"))
		{
			ReturnValue.Add("InitialStatus", JobInfoToUse->bSubmitJobAsSuspended ? "Suspended" : "Active");
		}
		else if (PropertyName.IsEqual("bMachineListIsADenyList"))
		{
			ReturnValue.Add(JobInfoToUse->bMachineListIsADenyList ? "Denylist" : "Allowlist", JobInfoToUse->MachineList);
		}
		else if (PropertyName.IsEqual("PreJobScript"))
		{
			ReturnValue.Add(PropertyName.ToString(), JobInfoToUse->PreJobScript.FilePath);
		}
		else if (PropertyName.IsEqual("PostJobScript"))
		{
			ReturnValue.Add(PropertyName.ToString(), JobInfoToUse->PostJobScript.FilePath);
		}
		else if (PropertyName.IsEqual("PreTaskScript"))
		{
			ReturnValue.Add(PropertyName.ToString(), JobInfoToUse->PreTaskScript.FilePath);
		}
		else if (PropertyName.IsEqual("PostTaskScript"))
		{
			ReturnValue.Add(PropertyName.ToString(), JobInfoToUse->PostTaskScript.FilePath);
		}
		else if (PropertyName.IsEqual("MachineList") || PropertyName.IsEqual("PluginInfoPreset"))
		{
			// MachineList is handled above, PluginInfoPreset is handled in a separate function
			continue;
		}
		else if (const FMapProperty* MapProperty = CastField<FMapProperty>(Property))
		{
			// Custom handler for Maps
			const void* MapValuePtr = MapProperty->ContainerPtrToValuePtr<void>(JobInfoToUse);
			FScriptMapHelper MapHelper(MapProperty, MapValuePtr);
			for (int32 MapSparseIndex = 0; MapSparseIndex < MapHelper.GetMaxIndex(); ++MapSparseIndex)
			{
				if (MapHelper.IsValidIndex(MapSparseIndex))
				{
					const uint8* MapKeyData = MapHelper.GetKeyPtr(MapSparseIndex);
					const uint8* MapValueData = MapHelper.GetValuePtr(MapSparseIndex);

					FString KeyDataAsString;
					MapHelper.GetKeyProperty()->ExportText_Direct(KeyDataAsString, MapKeyData, MapKeyData, nullptr, PPF_None);
					FString ValueDataAsString;
					MapHelper.GetValueProperty()->ExportText_Direct(ValueDataAsString, MapValueData, MapValueData, nullptr, PPF_None);

					FString PropertyNameAsString = FString::Printf(TEXT("%s%d"), *PropertyName.ToString(), MapSparseIndex);
					FString PropertyValueAsString = FString::Printf(TEXT("%s=%s"), *KeyDataAsString, *ValueDataAsString);
					ReturnValue.Add(PropertyNameAsString, PropertyValueAsString);
					UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *PropertyNameAsString, *PropertyValueAsString);
				}
			}
		}
		else
		{
			const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(JobInfoToUse);
			FString PropertyNameAsString = PropertyName.ToString();
			FString PropertyValueAsString;
			Property->ExportText_Direct(PropertyValueAsString, ValuePtr, ValuePtr, nullptr, PPF_None);

			if (PropertyValueAsString.TrimStartAndEnd().IsEmpty())
			{
				continue;
			}

			// Sanitize bool
			if (Property->IsA(FBoolProperty::StaticClass()))
			{
				PropertyNameAsString.RemoveFromStart(TEXT("b"), ESearchCase::CaseSensitive);
				PropertyValueAsString = PropertyValueAsString.ToLower();
			}
	
			ReturnValue.Add(PropertyNameAsString, PropertyValueAsString);
			UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *PropertyNameAsString, *PropertyValueAsString);
		}
	}

	return ReturnValue;
}

TMap<FString, FString> UDeadlineJobPresetLibrary::GetPluginInfo()
{
	return JobInfo.PluginInfoPreset;
}
