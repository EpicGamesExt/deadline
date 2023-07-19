// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoviePipelineDeadline.h"

#include "DeadlineJobPresetLibraryCustomization.h"
#include "DeadlineServiceEditorSettings.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

void FMoviePipelineDeadlineModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		FDeadlineJobInfoStruct::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDeadlineJobPresetLibraryCustomization::MakeInstance));

	PropertyModule.RegisterCustomClassLayout(
		UMoviePipelineDeadlineExecutorJob::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FMoviePipelineDeadlineExecutorJobCustomization::MakeInstance));

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FMoviePipelineDeadlineModule::ShutdownModule()
{
	if (FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		PropertyModule->UnregisterCustomPropertyTypeLayout(FDeadlineJobInfoStruct::StaticStruct()->GetFName());
		PropertyModule->UnregisterCustomPropertyTypeLayout(UMoviePipelineDeadlineExecutorJob::StaticClass()->GetFName());

		PropertyModule->NotifyCustomizationModuleChanged();
	}
}

UMoviePipelineDeadlineExecutorJob::UMoviePipelineDeadlineExecutorJob()
		: UMoviePipelineExecutorJob()
{
	if (const UMoviePipelineDeadlineSettings* MpdSettings = GetDefault<UMoviePipelineDeadlineSettings>())
	{
		if (UDeadlineServiceEditorSettings* DeadlineServiceSettings = GetMutableDefault<UDeadlineServiceEditorSettings>())
		{
			if (FMoviePipelineDeadlineOverrideSettings* Overrides = DeadlineServiceSettings->GetPresetOverrideSettings())
			{
				TObjectPtr<UDeadlineJobPresetLibrary> DefaultPreset = MpdSettings->DefaultPresetLibrary;
				TObjectPtr<UDeadlineJobPresetLibrary> CurrentPreset = Overrides->PresetLibrary;
				if (!DefaultPreset || CurrentPreset)
				{
					return;
				}
				
				DeadlineServiceSettings->GetPresetOverrideSettings()->PresetLibrary = DefaultPreset;
			}
		}
	}
}

TSharedRef<IDetailCustomization> FMoviePipelineDeadlineExecutorJobCustomization::MakeInstance()
{
	return MakeShared<FMoviePipelineDeadlineExecutorJobCustomization>();
}

void FMoviePipelineDeadlineExecutorJobCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Append override options from Settings object, if it exists
	if (UDeadlineServiceEditorSettings* DeadlineServiceSettings = GetMutableDefault<UDeadlineServiceEditorSettings>())
	{
		const TArray<UObject*> Objects = {DeadlineServiceSettings};
		const TSharedPtr<IPropertyHandle> PropertyHandle = DetailBuilder.AddObjectPropertyData(
			Objects, TEXT("PresetOverrideSettings"));

		const FName DeadlineCategoryName = TEXT("Deadline");
		IDetailCategoryBuilder& DeadlineCategory = DetailBuilder.EditCategory(
			DeadlineCategoryName, FText::FromName(DeadlineCategoryName));
		
		DeadlineCategory.AddProperty(PropertyHandle);
	}
}

IMPLEMENT_MODULE(FMoviePipelineDeadlineModule, MoviePipelineDeadline);
