#include "SplineAssetEditor.h"

#include "Fonts/SlateFontInfo.h"
#include "Internationalization/Text.h"
#include "SplineAsset.h"
#include "UObject/Class.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#include "SplineAssetEditorSettings.h"


#define LOCTEXT_NAMESPACE "SplineAssetEditor"


/* STextAssetEditor interface
 *****************************************************************************/

SplineAssetEditor::~SplineAssetEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SplineAssetEditor::Construct(const FArguments& InArgs, USplineAsset* InSplineAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	SplineAsset = InSplineAsset;

	auto Settings = GetDefault<USplineAssetEditorSettings>();

	ChildSlot
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(EditableTextBox, SMultiLineEditableTextBox)
			.BackgroundColor((Settings != nullptr) ? Settings->BackgroundColor : FLinearColor::White)
		.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
		.ForegroundColor((Settings != nullptr) ? Settings->ForegroundColor : FLinearColor::Black)
		.Margin((Settings != nullptr) ? Settings->Margin : 4.0f)
		.OnTextChanged(this, &SplineAssetEditor::HandleEditableTextBoxTextChanged)
		.OnTextCommitted(this, &SplineAssetEditor::HandleEditableTextBoxTextCommitted)
		.Text(SplineAsset->SplineJSON)
		]
		];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SplineAssetEditor::HandleTextAssetPropertyChanged);
}


/* STextAssetEditor callbacks
 *****************************************************************************/

void SplineAssetEditor::HandleEditableTextBoxTextChanged(const FText& NewText)
{
	SplineAsset->MarkPackageDirty();
}


void SplineAssetEditor::HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType)
{
	SplineAsset->SplineJSON = EditableTextBox->GetText();
}


void SplineAssetEditor::HandleTextAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == SplineAsset)
	{
		EditableTextBox->SetText(SplineAsset->SplineJSON);
	}
}


#undef LOCTEXT_NAMESPACE