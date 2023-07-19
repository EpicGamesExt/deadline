// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Engine/DeveloperSettings.h"
#include "MoviePipelineQueue.h"
#include "DeadlineJobDataAsset.h"
#include "IDetailCustomization.h"
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
class UMoviePipelineDeadlineExecutorJob : public UMoviePipelineExecutorJob
{
	GENERATED_BODY()
public:
	UMoviePipelineDeadlineExecutorJob();
};

class FMoviePipelineDeadlineExecutorJobCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;
};
