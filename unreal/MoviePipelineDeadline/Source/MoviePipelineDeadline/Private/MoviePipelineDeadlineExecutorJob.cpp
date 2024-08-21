// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoviePipelineDeadlineExecutorJob.h"

#include "DeadlineJobPreset.h"
#include "MoviePipelineDeadlineSettings.h"

#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogMoviePipelineDeadline, Log, All)

namespace UE::MoviePipelineDeadlineExecutorJob::Private
{
	TObjectPtr<UDeadlineJobPreset> GetDefaultJobPreset()
	{
		if (const UMoviePipelineDeadlineSettings* MpdSettings = GetDefault<UMoviePipelineDeadlineSettings>())
		{
			if (const TObjectPtr<UDeadlineJobPreset> DefaultPreset = MpdSettings->DefaultJobPreset)
			{
				return DefaultPreset;
			}
		}
		return nullptr;
	}

	FDeadlineJobPresetStruct GetDeadlineJobPresetStruct(TObjectPtr<UDeadlineJobPreset> InPreset)
	{
		TObjectPtr<UDeadlineJobPreset> Preset = InPreset ? InPreset : GetDefaultJobPreset();
		if (IsValid(Preset))
		{
			return InPreset->JobPresetStruct;
		}

		UE_LOG(LogMoviePipelineDeadline, Warning, TEXT("Provided Job Preset is empty! Using the default deadline preset struct."));
		UDeadlineJobPreset* DefaultJob = Cast<UDeadlineJobPreset>(UDeadlineJobPreset::StaticClass()->GetDefaultObject(true));
		return DefaultJob->JobPresetStruct;
	}
}

UMoviePipelineDeadlineExecutorJob::UMoviePipelineDeadlineExecutorJob()
		: UMoviePipelineExecutorJob()
{
	// If a Job Preset is not already defined, assign the default preset
	if (!JobPreset)
	{
		JobPreset = UE::MoviePipelineDeadlineExecutorJob::Private::GetDefaultJobPreset();
	}
}

bool UMoviePipelineDeadlineExecutorJob::IsPropertyRowEnabledInMovieRenderJob(const FName& InPropertyPath) const
{
	if (const FPropertyRowEnabledInfo* Match = Algo::FindByPredicate(EnabledPropertyOverrides,
		 [&InPropertyPath](const FPropertyRowEnabledInfo& Info)
		 {
			 return Info.PropertyPath == InPropertyPath;
		 }))
	{
		return Match->bIsEnabled;
	}

	return false;
}

void UMoviePipelineDeadlineExecutorJob::SetPropertyRowEnabledInMovieRenderJob(const FName& InPropertyPath, bool bInEnabled)
{
	if (FPropertyRowEnabledInfo* Match = Algo::FindByPredicate(EnabledPropertyOverrides,
		 [&InPropertyPath](const FPropertyRowEnabledInfo& Info)
		 {
			 return Info.PropertyPath == InPropertyPath;
		 }))
	{
		Match->bIsEnabled = bInEnabled;
	}
	else
	{
		EnabledPropertyOverrides.Add({InPropertyPath, bInEnabled});
	}
}

void UMoviePipelineDeadlineExecutorJob::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Check if we changed the job Preset an update the override details
	if (const FName PropertyName = PropertyChangedEvent.GetPropertyName(); PropertyName == "JobPreset")
	{
		if (const UDeadlineJobPreset* SelectedJobPreset = this->JobPreset)
		{
			this->PresetOverrides = SelectedJobPreset->JobPresetStruct;
		}
	}
}

FDeadlineJobPresetStruct UMoviePipelineDeadlineExecutorJob::GetDeadlineJobPresetStructWithOverrides() const
{
	// Start with preset properties
	FDeadlineJobPresetStruct ReturnValue = UE::MoviePipelineDeadlineExecutorJob::Private::GetDeadlineJobPresetStruct(JobPreset);
	
	const UMoviePipelineDeadlineSettings* Settings = GetDefault<UMoviePipelineDeadlineSettings>();

	for (TFieldIterator<FProperty> PropIt(FDeadlineJobPresetStruct::StaticStruct()); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		if (!Property)
		{
			continue;
		}

		const FName PropertyPath = *Property->GetPathName();

		// Skip hidden properties (just return the preset value)
		if (Settings && Settings->GetIsPropertyHiddenInMovieRenderQueue(PropertyPath))
		{
			continue;
		}

		// Also skip if it's shown but not enabled
		if (!IsPropertyRowEnabledInMovieRenderJob(PropertyPath))
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
