#include "SplineAssetFactory.h"

#include "Containers/UnrealString.h"
#include "SplineAsset.h"
#include "Misc/FileHelper.h"

class USplineAsset;

#define LOCTEXT_NAMESPACE "Mjolnir" 

/* UTextAssetFactory structors
 *****************************************************************************/

USplineAssetFactory::USplineAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Formats.Add(FString(TEXT("txt;")) + NSLOCTEXT("USplineAssetFactory", "FormatTxt", "Spline File").ToString());
	SupportedClass = USplineAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
}


/* UFactory overrides
 *****************************************************************************/

 /* This is the old API (only for demonstration purposes)
 UObject* UTextAssetFactory::FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn)
 {
	 UTextAsset* TextAsset = nullptr;
	 FString TextString;
	 if (FFileHelper::LoadFileToString(TextString, *CurrentFilename))
	 {
		 TextAsset = NewObject<UTextAsset>(InParent, Class, Name, Flags);
		 TextAsset->Text = FText::FromString(TextString);
	 }
	 return TextAsset;
 }*/

UObject* USplineAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<USplineAsset>(InParent, InClass, InName, Flags);
}

UObject* USplineAssetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	USplineAsset* SplineAsset = nullptr;
	FString TextString;

	if (FFileHelper::LoadFileToString(TextString, *Filename))
	{
		SplineAsset = NewObject<USplineAsset>(InParent, InClass, InName, Flags);
		SplineAsset->SplineJSON = FText::FromString(TextString);
	}

	bOutOperationCanceled = false;

	return SplineAsset;
}

uint32 USplineAssetFactory::GetMenuCategories() const
{
	return FMjolnir::Get().SplinesAssetCategory;
}

FText USplineAssetFactory::GetDisplayName() const
{
	return LOCTEXT("SplineText", "Spline");
}

FString USplineAssetFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("NewSpline"));
}

bool USplineAssetFactory::ShouldShowInNewMenu() const
{
	return true;
}

#undef LOCTEXT_NAMESPACE