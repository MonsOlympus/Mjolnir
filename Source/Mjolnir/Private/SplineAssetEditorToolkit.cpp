#include "SplineAssetEditorToolkit.h"

#include "Editor.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "SplineAssetEditor.h"
#include "SplineAsset.h"
#include "UObject/NameTypes.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FSplineAssetEditorToolkit"

DEFINE_LOG_CATEGORY_STATIC(LogSplineAssetEditor, Log, All);


/* Local constants
 *****************************************************************************/

namespace FSplineAssetEditor
{
	static const FName AppIdentifier("SplineAssetEditorApp");
	static const FName TabId("SplineEditor");
}


/* FTextAssetEditorToolkit structors
 *****************************************************************************/

FSplineAssetEditorToolkit::FSplineAssetEditorToolkit(const TSharedRef<ISlateStyle>& InStyle)
	: SplineAsset(nullptr)
	, Style(InStyle)
{ }


FSplineAssetEditorToolkit::~FSplineAssetEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	GEditor->UnregisterForUndo(this);
}

/* FTextAssetEditorToolkit interface
 *****************************************************************************/

void FSplineAssetEditorToolkit::Initialize(USplineAsset* InSplineAsset, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost)
{
	SplineAsset = InSplineAsset;

	// Support undo/redo
	SplineAsset->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);

	// create tab layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_SplineAssetEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.66f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.1f)

				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(FSplineAssetEditor::TabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.9f)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(
		InMode,
		InToolkitHost,
		FSplineAssetEditor::AppIdentifier,
		Layout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InSplineAsset
	);

	RegenerateMenusAndToolbars();
}


/* FAssetEditorToolkit interface
 *****************************************************************************/

FString FSplineAssetEditorToolkit::GetDocumentationLink() const
{
	return FString(TEXT(""));// FString(TEXT("https://github.com/ue4plugins/TextAsset"));
}


void FSplineAssetEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_SplineAssetEditor", "Spline Asset Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(FSplineAssetEditor::TabId, FOnSpawnTab::CreateSP(this, &FSplineAssetEditorToolkit::HandleTabManagerSpawnTab, FSplineAssetEditor::TabId))
		.SetDisplayName(LOCTEXT("TextEditorTabName", "Text Editor"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
}


void FSplineAssetEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FSplineAssetEditor::TabId);
}


/* IToolkit interface
 *****************************************************************************/

FText FSplineAssetEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Spline Asset Editor");
}


FName FSplineAssetEditorToolkit::GetToolkitFName() const
{
	return FName("SplineAssetEditor");
}


FLinearColor FSplineAssetEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}


FString FSplineAssetEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "SplineAsset ").ToString();
}


/* FGCObject interface
 *****************************************************************************/

void FSplineAssetEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(SplineAsset);
}


/* FEditorUndoClient interface
*****************************************************************************/

void FSplineAssetEditorToolkit::PostUndo(bool bSuccess)
{ }


void FSplineAssetEditorToolkit::PostRedo(bool bSuccess)
{
	PostUndo(bSuccess);
}


/* FTextAssetEditorToolkit callbacks
 *****************************************************************************/

TSharedRef<SDockTab> FSplineAssetEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if (TabIdentifier == FSplineAssetEditor::TabId)
	{
		TabWidget = SNew(SplineAssetEditor, SplineAsset, Style);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}


#undef LOCTEXT_NAMESPACE