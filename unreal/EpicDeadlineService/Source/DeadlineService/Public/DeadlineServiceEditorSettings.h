// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "DeadlineServiceEditorSettings.generated.h"

/**
* Project-wide settings for the Deadline Service.
*/
UCLASS(BlueprintType, config = Editor, defaultconfig, meta = (DisplayName = "Deadline Service"))
class DEADLINESERVICE_API UDeadlineServiceEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UDeadlineServiceEditorSettings();
	
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

protected:

	UPROPERTY(config)
	TArray<FName> PropertiesToHideInMovieRenderQueue;

};
