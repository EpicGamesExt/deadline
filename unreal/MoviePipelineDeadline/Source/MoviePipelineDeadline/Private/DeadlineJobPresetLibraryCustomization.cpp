// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlineJobPresetLibraryCustomization.h"

#include "DeadlineServiceEditorSettings.h"
#include "MoviePipelineDeadline.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailGroup.h"
#include "Widgets/Input/SCheckBox.h"

class SEyeCheckBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SEyeCheckBox ){}

	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, const FName& InPropertyPath)
	{		
		ChildSlot
		[
			SNew(SBox)
			.Visibility(EVisibility::Visible)
			.HAlign(HAlign_Right)
			.WidthOverride(28)
			.HeightOverride(20)
			.Padding(4, 0)
			[
				SAssignNew(CheckBoxPtr, SCheckBox)
				.Style(&FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>("ToggleButtonCheckbox"))
				.Visibility_Lambda([this]()
				{
					return CheckBoxPtr.IsValid() && !CheckBoxPtr->IsChecked() ? EVisibility::Visible : IsHovered() ? EVisibility::Visible : EVisibility::Hidden;
				})
				.CheckedImage(FAppStyle::Get().GetBrush("Icons.Visible"))
				.CheckedHoveredImage(FAppStyle::Get().GetBrush("Icons.Visible"))
				.CheckedPressedImage(FAppStyle::Get().GetBrush("Icons.Visible"))
				.UncheckedImage(FAppStyle::Get().GetBrush("Icons.Hidden"))
				.UncheckedHoveredImage(FAppStyle::Get().GetBrush("Icons.Hidden"))
				.UncheckedPressedImage(FAppStyle::Get().GetBrush("Icons.Hidden"))
				.ToolTipText(NSLOCTEXT("FDeadlineJobPresetLibraryCustomization", "VisibleInMoveRenderQueueToolTip", "If true this property will be visible for overriding from Movie Render Queue."))
				.OnCheckStateChanged_Lambda([InPropertyPath](ECheckBoxState CheckType)
				{
					if (UDeadlineServiceEditorSettings* Settings =
						GetMutableDefault<UDeadlineServiceEditorSettings>())
					{
						if (CheckType == ECheckBoxState::Unchecked)
						{
							Settings->AddPropertyToHideInMovieRenderQueue(
								InPropertyPath);
						}
						else
						{
							Settings->
								RemovePropertyToHideInMovieRenderQueue(
									InPropertyPath);
						}
					}
				})
				.IsChecked_Lambda([InPropertyPath]()
				{
					return FDeadlineJobPresetLibraryCustomization::IsPropertyHiddenInMovieRenderQueue(InPropertyPath)
								? ECheckBoxState::Unchecked
								: ECheckBoxState::Checked;
				})
			]
		];
	}

	TSharedPtr<SCheckBox> CheckBoxPtr;
};

TSharedRef<IPropertyTypeCustomization> FDeadlineJobPresetLibraryCustomization::MakeInstance()
{
	return MakeShared<FDeadlineJobPresetLibraryCustomization>();
}

void FDeadlineJobPresetLibraryCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	TArray<UObject*> OuterObjects;
	StructHandle->GetOuterObjects(OuterObjects);

	if (OuterObjects.Num() == 0)
	{
		return;
	}

	const TWeakObjectPtr<UObject> OuterObject = OuterObjects[0];
	if (!OuterObject.IsValid())
	{
		return;
	}
	const bool bIsOverrideType = OuterObject->IsA(UMoviePipelineDeadlineExecutorJob::StaticClass());

	TMap<FName, IDetailGroup*> CreatedCategories;

	const FName StructName(StructHandle->GetProperty()->GetFName());

	if (bIsOverrideType)
	{
		IDetailGroup& BaseCategoryGroup = ChildBuilder.AddGroup(StructName, StructHandle->GetPropertyDisplayName());
		CreatedCategories.Add(StructName, &BaseCategoryGroup);
	}
	
	// For each map member and each struct member in the map member value
	uint32 NumChildren;
	StructHandle->GetNumChildren(NumChildren);
	
	// For each struct member
	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef<IPropertyHandle> ChildHandle = StructHandle->GetChildHandle(ChildIndex).ToSharedRef();

		// Skip properties that are hidden so we don't end up creating empty categories in the job details
		if (bIsOverrideType && IsPropertyHiddenInMovieRenderQueue(*ChildHandle->GetProperty()->GetPathName()))
		{
			continue;
		}
		
		IDetailGroup* GroupToUse = nullptr;
		if (const FString* PropertyCategoryString = ChildHandle->GetProperty()->FindMetaData(TEXT("Category")))
		{
			FName PropertyCategoryName(*PropertyCategoryString);

			if (IDetailGroup** FoundCategory = CreatedCategories.Find(PropertyCategoryName))
			{
				GroupToUse = *FoundCategory;
			}
			else
			{
				if (bIsOverrideType)
				{
					IDetailGroup& NewGroup = CreatedCategories.FindChecked(StructName)->AddGroup(PropertyCategoryName, FText::FromName(PropertyCategoryName), true);
					GroupToUse = CreatedCategories.Add(PropertyCategoryName, &NewGroup);
				}
				else
				{
					IDetailGroup& NewGroup = ChildBuilder.AddGroup(PropertyCategoryName, FText::FromName(PropertyCategoryName));
					NewGroup.ToggleExpansion(true);
					GroupToUse = CreatedCategories.Add(PropertyCategoryName, &NewGroup);
				}
			}
		}
		
		IDetailPropertyRow& PropertyRow = GroupToUse->AddPropertyRow(ChildHandle);

		if (bIsOverrideType)
		{
			CustomizeStructChildrenInMovieRenderQueue(PropertyRow, OuterObject);
		}
		else
		{
			CustomizeStructChildrenInAssetDetails(PropertyRow);
		}
	}

	// Force expansion of all categories
	for (const TTuple<FName, IDetailGroup*>& Pair : CreatedCategories)
	{
		if (Pair.Value)
		{
			Pair.Value->ToggleExpansion(true);
		}
	}
}

void FDeadlineJobPresetLibraryCustomization::CustomizeStructChildrenInAssetDetails(IDetailPropertyRow& PropertyRow)
{
	TSharedPtr<SWidget> NameWidget;
	TSharedPtr<SWidget> ValueWidget;
	FDetailWidgetRow Row;
	PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);

	PropertyRow.CustomWidget(true)
	.NameContent()
	.MinDesiredWidth(Row.NameWidget.MinWidth)
	.MaxDesiredWidth(Row.NameWidget.MaxWidth)
	.HAlign(HAlign_Fill)
	[
		NameWidget.ToSharedRef()
	]
	.ValueContent()
	.MinDesiredWidth(Row.ValueWidget.MinWidth)
	.MaxDesiredWidth(Row.ValueWidget.MaxWidth)
	.VAlign(VAlign_Center)
	[
		ValueWidget.ToSharedRef()
	]
	.ExtensionContent()
	[
		SNew(SEyeCheckBox, *PropertyRow.GetPropertyHandle()->GetProperty()->GetPathName())
	];
}

void FDeadlineJobPresetLibraryCustomization::CustomizeStructChildrenInMovieRenderQueue(IDetailPropertyRow& PropertyRow, TWeakObjectPtr<UObject> OuterObject)
{	
	TSharedPtr<SWidget> NameWidget;
	TSharedPtr<SWidget> ValueWidget;
	FDetailWidgetRow Row;
	PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);
	
	PropertyRow
	.OverrideResetToDefault(
		FResetToDefaultOverride::Create(
			FIsResetToDefaultVisible::CreateSP(this, &FDeadlineJobPresetLibraryCustomization::IsResetToDefaultVisibleOverride, OuterObject),
			FResetToDefaultHandler::CreateSP(this, &FDeadlineJobPresetLibraryCustomization::ResetToDefaultOverride, OuterObject)))
	.CustomWidget(true)
	.NameContent()
	.MinDesiredWidth(Row.NameWidget.MinWidth)
	.MaxDesiredWidth(Row.NameWidget.MaxWidth)
	.HAlign(HAlign_Fill)
	[
		NameWidget.ToSharedRef()
	]
	.ValueContent()
	.MinDesiredWidth(Row.ValueWidget.MinWidth)
	.MaxDesiredWidth(Row.ValueWidget.MaxWidth)
	.VAlign(VAlign_Center)
	[
		ValueWidget.ToSharedRef()
	];
}

bool FDeadlineJobPresetLibraryCustomization::IsPropertyHiddenInMovieRenderQueue(const FName& InPropertyPath)
{
	if (const UDeadlineServiceEditorSettings* Settings = GetDefault<UDeadlineServiceEditorSettings>())
	{
		return Settings->GetIsPropertyHiddenInMovieRenderQueue(InPropertyPath);
	}
	return false;
}

bool GetPresetValueAsString(const FProperty* PropertyPtr, TWeakObjectPtr<UObject> OuterObject, FString& OutFormattedValue)
{
	if (!PropertyPtr || !OuterObject.IsValid())
	{
		return false;
	}

	const UMoviePipelineDeadlineExecutorJob* DeadlineExecutorJob = Cast<UMoviePipelineDeadlineExecutorJob>(OuterObject.Get());
	if (!DeadlineExecutorJob)
	{
		return false;
	}

	UDeadlineJobPresetLibrary* SelectedPresetLibrary = DeadlineExecutorJob->PresetLibrary;
	if (!SelectedPresetLibrary)
	{
		return false;
	}

	const void* ValuePtr = PropertyPtr->ContainerPtrToValuePtr<void>(&SelectedPresetLibrary->JobInfo);
	PropertyPtr->ExportText_Direct(OutFormattedValue, ValuePtr, ValuePtr, nullptr, PPF_None);
	return true;
}

bool FDeadlineJobPresetLibraryCustomization::IsResetToDefaultVisibleOverride(
	TSharedPtr<IPropertyHandle> PropertyHandle, TWeakObjectPtr<UObject> OuterObject)
{
	if (!PropertyHandle || !OuterObject.IsValid())
	{
		return true;
	}
	
	if (FString DefaultValueAsString; GetPresetValueAsString(PropertyHandle->GetProperty(), OuterObject, DefaultValueAsString))
	{
		FString CurrentValueAsString;
		PropertyHandle->GetValueAsFormattedString(CurrentValueAsString);

		return CurrentValueAsString != DefaultValueAsString; 
	}

	// If this fails, just show it by default
	return true;
}

void FDeadlineJobPresetLibraryCustomization::ResetToDefaultOverride(
	TSharedPtr<IPropertyHandle> PropertyHandle, TWeakObjectPtr<UObject> OuterObject)
{
	if (!PropertyHandle || !OuterObject.IsValid())
	{
		return;
	}
	
	if (FString DefaultValueAsString; GetPresetValueAsString(PropertyHandle->GetProperty(), OuterObject, DefaultValueAsString))
	{
		PropertyHandle->SetValueFromFormattedString(DefaultValueAsString);
	}
}
