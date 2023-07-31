// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Engine/DataAsset.h"
#include "DeadlineServiceEditorSettings.h"
#include "DeadlineJobDataAsset.generated.h"

// Forward declarations
class UDeadlineJobPresetLibrary;
class UScriptCategories;

DECLARE_LOG_CATEGORY_EXTERN(LogDeadlineDataAsset, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogDeadlineStruct, Log, All);


/*
 * Deadline Pre Post script struct
 */
USTRUCT(BlueprintType)
struct FPrePostScriptStruct
{
	GENERATED_BODY()

	/** Script Category from project settings pre post script mapping */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions="DeadlineService.ScriptCategories.GetScriptCategories"), Category = "Scripts")
	FString ScriptCategory;

	/** Name of the script to execute */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scripts")
	FString ScriptName;

};


/**
 * A class to return the script categories from the project settings
 */
UCLASS()
class UScriptCategories : public UObject
{
	GENERATED_BODY()
public:

	/** Returns a list of available script categories.
	 * To make this available to the GetOptions meta tag on a property, we need to make this a UFUNCTION
	* Note: This list generally will come from Deadline */
	UFUNCTION()
	static TArray<FString> GetScriptCategories()
	{
		const UDeadlineServiceEditorSettings* EditorSettings = GetDefault<UDeadlineServiceEditorSettings>();

		TArray<FString> ScriptCategories;

		if (EditorSettings->ScriptCategoryMappings.Num() > 0)
		{
			EditorSettings->ScriptCategoryMappings.GenerateKeyArray(ScriptCategories);
		}
		
		return ScriptCategories;
	}

};

/**
 * Deadline Job Info Struct
 */
USTRUCT(BlueprintType)
struct FDeadlineJobInfoStruct
{
	GENERATED_BODY()

	/** Deadline Plugin used to execute the current job. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin")
	FString Plugin = TEXT("UnrealEngine5");

	/** Deadline Auxiliary files to upload for the current job */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aux Files")
	TArray<FFilePath> AuxFiles;

	/** Job BatchName. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString BatchName;

    /** Job name. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString Name;

    /** Job comments. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString Comment;

    /** Job department. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString Department;

	/** Job pool. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString Pool;

	/** Secondary job pool. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Options")
	FString SecondaryPool;

	/** Job group. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString Group;

	/** Job Frames. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString Frames = TEXT("0");

	/** Job user. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Options")
	FString UserName;

	/** Pre-Job python render script.
	 * Note:
	 * Select the location and specify the name of the script. Example: `custom_scripts_dir` my_pre_job_script.py
	 * This location is expected to already be path mapped on the farm else it will fail.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Scripts")
	FPrePostScriptStruct PreJobScript;

	/** Post-Job python render script.
	 * Note:
	 * Select the location and specify the name of the script. Example: `custom_scripts_dir` my_post_job_script.py
	 * This location is expected to already be path mapped on the farm else it will fail.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Scripts")
	FPrePostScriptStruct PostJobScript;

	/** Pre-Task python render script.
	 * Note:
	 * Select the location and specify the name of the script. Example: `custom_scripts_dir` my_pre_task_script.py
	 * This location is expected to already be path mapped on the farm else it will fail.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Scripts")
	FPrePostScriptStruct PreTaskScript;

	/** Post-Job python render script.
	 * Note:
	 * Select the location and specify the name of the script. Example: `custom_scripts_dir` my_post_task_script.py
	 * This location is expected to already be path mapped on the farm else it will fail.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Scripts")
	FPrePostScriptStruct PostTaskScript;

	/** Key Value pair environment variables to set when the job renders. This is only set in the Deadline environment not the Unreal environment. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Environment Variables")
	TMap<FString, FString> EnvironmentKeyValue;

	/** Job Extra Info keys used for storing user data on the job. This is split up into unique
	 * settings as there is a limited amount of settings  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	FString ExtraInfo9;

	/** Key-Value pair Job Extra Info keys for storing user data on the job. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Job Extra Info")
	TMap<FString, FString> ExtraInfoKeyValue;

	/** Replace the Task extra info column names with task extra info value. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	bool OverrideTaskExtraInfoNames = false;

	/**
	 *Key Value pair Task Extra Info keys for storing deadline info. This is split up into unique
	 * settings as there is a limited amount of settings
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Task Extra Info")
	FString TaskExtraInfoName9;

	/** Extra Deadline Job options. Note: Match the naming convention on Deadline's Manual Job Submission website for the options. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "Options")
	TMap<FString, FString> ExtraJobOptions;
	
};


/**
 * Deadline Plugin Info Struct
 */
USTRUCT(BlueprintType)
struct FDeadlinePluginInfoStruct
{
	GENERATED_BODY()

	/** Deadline Plugin info key value pair. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin Info")
	TMap<FString, FString> PluginInfo;
	
};

/*
 * Deadline job preset struct. Allows compiling similar job info and plugin info as a single entity.
 */
USTRUCT(BlueprintType)
struct FDeadlineJobPresetStruct
{
	GENERATED_BODY()

	/** Deadline Job Preset struct. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job Info")
	FDeadlineJobInfoStruct JobInfoPreset;

	/** Deadline plugin Preset struct. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plugin Info")
	FDeadlinePluginInfoStruct PluginInfoPreset;
	
};


/**
 * Deadline Job and Plugin Info Preset Library. This data asset saves a mapping of presets that can be
 * selected depending on the task needed to execute a job
 */
UCLASS(BlueprintType)
class UDeadlineJobPresetLibrary : public UDataAsset
{
	GENERATED_BODY()
public:

	/** Mapping of Deadline job presets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Job Presets")
	TMap<FString, FDeadlineJobPresetStruct> DeadlineJobPresets;

	/*
	 * Returns a job info from the specified preset library as a Json String
	 */
	UFUNCTION(BlueprintCallable, Category = "Job Info preset")
	FString GetJobInfoPresetJsonString(FString PresetName);

	/*
	 * Returns a plugin info from the specified preset library as a Json String
	 */
	UFUNCTION(BlueprintCallable, Category = "Plugin Info preset")
	FString GetPluginInfoPresetJsonString(FString PresetName);
	
};
