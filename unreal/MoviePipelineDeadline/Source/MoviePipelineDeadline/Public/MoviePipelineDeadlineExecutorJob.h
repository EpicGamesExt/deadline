// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MoviePipelineQueue.h"
#include "DeadlineJobDataAsset.h"

#include "MoviePipelineDeadlineExecutorJob.generated.h"

USTRUCT()
struct FPropertyRowEnabledInfo
{
	GENERATED_BODY()
	
	FName PropertyPath;
	bool bIsEnabled = false;
};

UCLASS(BlueprintType, config = EditorPerProjectUserSettings)
class MOVIEPIPELINEDEADLINE_API UMoviePipelineDeadlineExecutorJob : public UMoviePipelineExecutorJob
{
	GENERATED_BODY()
public:
	UMoviePipelineDeadlineExecutorJob();

	bool IsPropertyRowEnabledInMovieRenderJob(const FName& InPropertyPath) const;

	void SetPropertyRowEnabledInMovieRenderJob(const FName& InPropertyPath, bool bInEnabled);

	/**
	 * Returns the Deadline job info with overrides applied, if enabled.
	 * Skips any property not 
	 */
	UFUNCTION(BlueprintCallable, Category = "DeadlineService")
	FDeadlineJobPresetStruct GetDeadlineJobInfoStructWithOverridesIfApplicable() const;

	/** `Batch Name` groups similar jobs together in the Deadline Monitor UI. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString BatchName;

	/* Deadline Preset Library. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deadline")
	TObjectPtr<UDeadlineJobPreset> PresetLibrary;

	/* Output directory override on Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FDirectoryPath OutputDirectoryOverride;

	/* Filename Format override on Deadline. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString FilenameFormatOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FDeadlineJobPresetStruct PresetOverrides = FDeadlineJobPresetStruct();

protected:

	UPROPERTY(config)
	TArray<FPropertyRowEnabledInfo> EnabledPropertyOverrides;
};
