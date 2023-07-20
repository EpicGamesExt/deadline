// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlineServiceEditorSettings.h"

#include "DeadlineJobDataAsset.h"

UDeadlineServiceEditorSettings::UDeadlineServiceEditorSettings()
{
	const TArray<FString> PropertiesToShowByDefault = {"Name", "Comment", "Department", "Pool", "Group", "Priority"};
	
	// Set up default properties to show in MRQ
	// We do this by setting everything to hide except some defined exceptions by name
	for (TFieldIterator<FProperty> PropIt(FDeadlineJobInfoStruct::StaticStruct()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		if (!Property)
		{
			continue;
		}

		if (PropertiesToShowByDefault.Contains(Property->GetName()))
		{
			continue;
		}
		
		PropertiesToHideInMovieRenderQueue.Add(*Property->GetPathName());
	}
}
