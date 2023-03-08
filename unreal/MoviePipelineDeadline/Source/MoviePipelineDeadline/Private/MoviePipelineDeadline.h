// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Engine/DeveloperSettings.h"
#include "MoviePipelineQueue.h"
#include "DeadlineJobDataAsset.h"
#include "MoviePipelineDeadline.generated.h"

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

	/** The project level Deadline preset library Name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, meta = (GetOptions=GetPresetNames), Category = "Movie Pipeline Deadline")
	FString DefaultPresetName;

private:
	/*
	 * Function to return the keys in the current preset library.
	 * This is used by the meta tag to generate a combobox for the preset name
	 */
	UFUNCTION()
	TArray<FString> GetPresetNames() const
	{
		TArray<FString> PresetNames;

		if (this->DefaultPresetLibrary)
		{
			this->DefaultPresetLibrary->DeadlineJobPresets.GenerateKeyArray(PresetNames);
		}

		return PresetNames;
	}
	
};

UCLASS(BlueprintType, config = EditorPerProjectUserSettings)
class UMoviePipelineDeadlineExecutorJob : public UMoviePipelineExecutorJob
{
	GENERATED_BODY()
public:
	UMoviePipelineDeadlineExecutorJob()
		: UMoviePipelineExecutorJob()
	{}
	
	/** `Batch Name` groups similar jobs together in the Deadline Monitor UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString BatchName;

	/* Deadline Preset Library. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deadline")
	TObjectPtr<UDeadlineJobPresetLibrary> PresetLibrary;

	/* Deadline Job Info and Plugin Info Preset Name. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, meta = (GetOptions=GetPresetNames), Category = "Deadline")
	FString PresetName;

	/* Output directory override on Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FDirectoryPath OutputDirectoryOverride;

	/* Filename Format override on Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString FilenameFormatOverride;

private:
	/*
 * Function to return the keys in the current preset library.
 * This is used by the meta tag to generate a combobox for the preset name
 */
	UFUNCTION()
	TArray<FString> GetPresetNames() const
	{
		TArray<FString> PresetNames;

		if (this->PresetLibrary)
		{
			this->PresetLibrary->DeadlineJobPresets.GenerateKeyArray(PresetNames);
		}

		return PresetNames;
	}
	
	
};

