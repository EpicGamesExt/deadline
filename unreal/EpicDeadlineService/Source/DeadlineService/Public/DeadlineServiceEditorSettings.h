// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DeadlineJobDataAsset.h"

#include "Engine/DeveloperSettings.h"
#include "DeadlineServiceEditorSettings.generated.h"

USTRUCT(BlueprintType)
struct FMoviePipelineDeadlineOverrideSettings
{
	GENERATED_BODY()

	/** `Batch Name` groups similar jobs together in the Deadline Monitor UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString BatchName;

	/* Deadline Preset Library. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deadline")
	TObjectPtr<UDeadlineJobPresetLibrary> PresetLibrary;

	/* Output directory override on Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FDirectoryPath OutputDirectoryOverride;

	/* Filename Format override on Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString FilenameFormatOverride;

	/* If true, PresetOverrides will be applied when sending the job to Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	bool bShouldOverridePresetProperties = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline", meta = (EditConditionHides, EditCondition = "bShouldOverridePresetProperties"))
	FDeadlineJobInfoStruct PresetOverrides;
};

/**
* Project-wide settings for the Deadline Service.
*/
UCLASS(BlueprintType, config = Editor, defaultconfig, meta = (DisplayName = "Deadline Service"))
class DEADLINESERVICE_API UDeadlineServiceEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return FName("Project"); }
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return FName("Plugins"); }
	
	/** UObject interface */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		SaveConfig();
	}

	/**
	* What is the host name for the Deadline Server that the REST API is running on?
	* Only needs the host name and port (ie: http://localhost:port)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString DeadlineHost;

	/**
	* The name of the plugin to load in Deadline. Usually the default is used.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString PluginName = "UnrealEngine";

	/*
	 * A mapping of script categories to a directory on disk. This is used by dropdown menus
	 * so a full path does not have to be specified
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	TMap<FString, FString> ScriptCategoryMappings;

	void AddPropertyToHideInMovieRenderQueue(const FName& InPropertyPath)
	{
		PropertiesToHideInMovieRenderQueue.Add(InPropertyPath);
	}

	void RemovePropertyToHideInMovieRenderQueue(const FName& InPropertyPath)
	{
		PropertiesToHideInMovieRenderQueue.Remove(InPropertyPath);
	}

	bool GetIsPropertyHiddenInMovieRenderQueue(const FName& InPropertyPath) const
	{
		return PropertiesToHideInMovieRenderQueue.Contains(InPropertyPath);
	}

	FMoviePipelineDeadlineOverrideSettings* GetPresetOverrideSettings()
	{
		return &PresetOverrideSettings; 
	}

protected:

	UPROPERTY(config, meta=(ShowOnlyInnerProperties))
	FMoviePipelineDeadlineOverrideSettings PresetOverrideSettings;

	UPROPERTY(config)
	TArray<FName> PropertiesToHideInMovieRenderQueue;

};
