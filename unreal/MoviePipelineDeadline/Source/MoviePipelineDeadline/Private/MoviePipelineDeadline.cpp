// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoviePipelineDeadline.h"

#include "DeadlineJobPresetLibraryCustomization.h"
#include "DeadlineServiceEditorSettings.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

void FMoviePipelineDeadlineModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FDeadlineJobInfoStruct::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDeadlineJobPresetLibraryCustomization::MakeInstance));

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FMoviePipelineDeadlineModule::ShutdownModule()
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		PropertyModule->UnregisterCustomPropertyTypeLayout(FDeadlineJobInfoStruct::StaticStruct()->GetFName());

		PropertyModule->NotifyCustomizationModuleChanged();
	}
}

UMoviePipelineDeadlineExecutorJob::UMoviePipelineDeadlineExecutorJob()
		: UMoviePipelineExecutorJob()
{
	// If a PresetLibrary is not already defined, assign the default preset library
	if (!PresetLibrary)
	{
		if (const UMoviePipelineDeadlineSettings* MpdSettings = GetDefault<UMoviePipelineDeadlineSettings>())
		{
			if (const TObjectPtr<UDeadlineJobPresetLibrary> DefaultPreset = MpdSettings->DefaultPresetLibrary)
			{
				PresetLibrary = DefaultPreset;
			}
		}
	}
}

FDeadlineJobInfoStruct UMoviePipelineDeadlineExecutorJob::GetDeadlineJobInfoStructWithOverridesIfApplicable() const
{
	if (!bShouldOverridePresetProperties)
	{
		return PresetLibrary->JobInfo;
	}

	// Start with preset properties
	FDeadlineJobInfoStruct ReturnValue = PresetLibrary->JobInfo;
	
	const UDeadlineServiceEditorSettings* Settings = GetDefault<UDeadlineServiceEditorSettings>();

	for (TFieldIterator<FProperty> PropIt(FDeadlineJobInfoStruct::StaticStruct()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		if (!Property)
		{
			continue;
		}

		// Skip hidden properties (just return the preset value)
		if (Settings && Settings->GetIsPropertyHiddenInMovieRenderQueue(*Property->GetPathName()))
		{
			continue;
		}

		// Get Override Property Value
		const void* OverridePropertyValuePtr = Property->ContainerPtrToValuePtr<void>(&PresetOverrides);

		void* ReturnPropertyValuePtr = Property->ContainerPtrToValuePtr<void>(&ReturnValue);
		Property->CopyCompleteValue(ReturnPropertyValuePtr, OverridePropertyValuePtr);
	}

	return ReturnValue;
}

IMPLEMENT_MODULE(FMoviePipelineDeadlineModule, MoviePipelineDeadline);
