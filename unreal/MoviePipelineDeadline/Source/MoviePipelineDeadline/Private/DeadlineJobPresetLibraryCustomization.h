// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IPropertyTypeCustomization.h"

class IDetailPropertyRow;

class FDeadlineJobPresetLibraryCustomization : public IPropertyTypeCustomization
{
public:

	static TSharedRef< IPropertyTypeCustomization > MakeInstance();

	/** Begin IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override {}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	/** End IPropertyTypeCustomization interface */

	static bool IsPropertyHiddenInMovieRenderQueue(const FName& InPropertyPath);
	
protected:
	void CustomizeStructChildrenInAssetDetails(IDetailPropertyRow& PropertyRow);
	void CustomizeStructChildrenInMovieRenderQueue(IDetailPropertyRow& PropertyRow, TWeakObjectPtr<UObject> OuterObject);

	bool IsResetToDefaultVisibleOverride(TSharedPtr<IPropertyHandle> PropertyHandle, TWeakObjectPtr<UObject> OuterObject);
	void ResetToDefaultOverride(TSharedPtr<IPropertyHandle> PropertyHandle, TWeakObjectPtr<UObject> OuterObject);
};
