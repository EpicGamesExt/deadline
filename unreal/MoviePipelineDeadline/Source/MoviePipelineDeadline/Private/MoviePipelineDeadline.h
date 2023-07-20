// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Engine/DeveloperSettings.h"
#include "MoviePipelineQueue.h"
#include "DeadlineJobDataAsset.h"
#include "Modules/ModuleInterface.h"

#include "MoviePipelineDeadline.generated.h"

class FMoviePipelineDeadlineModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	UMoviePipelineDeadlineExecutorJob* GetDeadlineExecutorJob() const
	{
		return GetMutableDefault<UMoviePipelineDeadlineExecutorJob>();
	}

	const UMoviePipelineDeadlineExecutorJob* GetDeadlineExecutorJobConst() const
	{
		return GetDefault<UMoviePipelineDeadlineExecutorJob>();
	}
};

/**
* Project-wide settings for Deadline Movie Pipeline.
*/
UCLASS(BlueprintType, config = Editor, defaultconfig, meta = (DisplayName = "Movie Pipeline Deadline"))
class UMoviePipelineDeadlineSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return FName("Project"); }
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return FName("Plugins"); }

	/** The project level Deadline preset Data Asset */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movie Pipeline Deadline")
	TObjectPtr<UDeadlineJobPresetLibrary> DefaultPresetLibrary;	
};

UCLASS(BlueprintType, config = EditorPerProjectUserSettings)
class MOVIEPIPELINEDEADLINE_API UMoviePipelineDeadlineExecutorJob : public UMoviePipelineExecutorJob
{
	GENERATED_BODY()
public:
	UMoviePipelineDeadlineExecutorJob();

	/**
	 * Returns the Deadline job info with overrides applied, if enabled.
	 * Skips any property not 
	 */
	UFUNCTION(BlueprintCallable, Category = "DeadlineService")
	FDeadlineJobInfoStruct GetDeadlineJobInfoStructWithOverridesIfApplicable() const;

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
	FDeadlineJobInfoStruct PresetOverrides = FDeadlineJobInfoStruct();
};
