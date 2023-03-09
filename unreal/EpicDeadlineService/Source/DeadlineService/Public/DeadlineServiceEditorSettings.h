// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Engine/DeveloperSettings.h"
#include "DeadlineServiceEditorSettings.generated.h"

/**
* Project-wide settings for the Deadline Service.
*/
UCLASS(BlueprintType, config = Editor, defaultconfig, meta = (DisplayName = "Deadline Service"))
class UDeadlineServiceEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	
	/** Gets the settings container name for the settings, either Project or Editor */
	virtual FName GetContainerName() const override { return FName("Project"); }
	/** Gets the category for the settings, some high level grouping like, Editor, Engine, Game...etc. */
	virtual FName GetCategoryName() const override { return FName("Plugins"); }

	/**
	* What is the host name for the Deadline Server that the REST API is running on?
	* Only needs the host name and port (ie: http://localhost:port)
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	FString DeadlineHost;

	/*
	 * A mapping of script categories to a directory on disk. This is used by dropdown menus
	 * so a full path does not have to be specified
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, config, Category = "Deadline")
	TMap<FString, FString> ScriptCategoryMappings;

};
