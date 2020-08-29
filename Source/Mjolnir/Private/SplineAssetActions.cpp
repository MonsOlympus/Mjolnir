#include "SplineAssetActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "SplineAsset.h"
#include "Styling/SlateStyle.h"

#include "SplineAssetEditorToolkit.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"

class USplineAsset;

/* FTextAssetActions constructors
 *****************************************************************************/

FSplineAssetActions::FSplineAssetActions(const TSharedRef<ISlateStyle>& InStyle)
	: Style(InStyle)
{ }


/* FAssetTypeActions_Base overrides
 *****************************************************************************/

bool FSplineAssetActions::CanFilter()
{
	return true;
}


void FSplineAssetActions::GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder)
{
	FAssetTypeActions_Base::GetActions(InObjects, MenuBuilder);

	auto SplineAssets = GetTypedWeakObjectPtrs<USplineAsset>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("SplineAsset_ReverseSpline", "Reverse Spline"),
		LOCTEXT("SplineAsset_ReverseSplineToolTip", "Reverse the spline stored in the selected text asset(s)."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateLambda([=] {
		for (auto& SplineAsset : SplineAssets)
		{
			if (SplineAsset.IsValid() && !SplineAsset->SplineJSON.IsEmpty())
			{
				SplineAsset->SplineJSON = FText::FromString(SplineAsset->SplineJSON.ToString().Reverse());
				SplineAsset->PostEditChange();
				SplineAsset->MarkPackageDirty();
			}
		}
	}),
			FCanExecuteAction::CreateLambda([=] {
		for (auto& SplineAsset : SplineAssets)
		{
			if (SplineAsset.IsValid() && !SplineAsset->SplineJSON.IsEmpty())
			{
				return true;
			}
		}
		return false;
	})
		)
	);
}


uint32 FSplineAssetActions::GetCategories()
{
	return FMjolnir::Get().SplinesAssetCategory;
	//return EAssetTypeCategories::Misc;
	//return EAssetTypeCategories::Splines;
}


FText FSplineAssetActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SplineAsset", "Spline Asset");
}


UClass* FSplineAssetActions::GetSupportedClass() const
{
	return USplineAsset::StaticClass();
}


FColor FSplineAssetActions::GetTypeColor() const
{
	return FColor::White;
}


bool FSplineAssetActions::HasActions(const TArray<UObject*>& InObjects) const
{
	return true;
}


void FSplineAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto SplineAsset = Cast<USplineAsset>(*ObjIt);

		if (SplineAsset != nullptr)
		{
			TSharedRef<FSplineAssetEditorToolkit> EditorToolkit = MakeShareable(new FSplineAssetEditorToolkit(Style));
			EditorToolkit->Initialize(SplineAsset, Mode, EditWithinLevelEditor);
		}
	}
}


#undef LOCTEXT_NAMESPACE