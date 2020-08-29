#pragma once

#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"
#include "UObject/ObjectMacros.h"

#include "SplineAssetEditorSettings.generated.h"


UCLASS(config = Editor)
class MJOLNIR_API USplineAssetEditorSettings : public UObject
{
	GENERATED_BODY()

public:

	/** Color of the TextAsset editor's background. */
	UPROPERTY(config, EditAnywhere, Category = Appearance)
	FSlateColor BackgroundColor;

	/** Color of the TextAsset editor's text. */
	UPROPERTY(config, EditAnywhere, Category = Appearance)
	FSlateColor ForegroundColor;

	/** The font to use in the TextAsset editor window. */
	UPROPERTY(config, EditAnywhere, Category = Appearance)
	FSlateFontInfo Font;

	/** The margin around the TextAsset editor window (in pixels). */
	UPROPERTY(config, EditAnywhere, Category = Appearance)
	float Margin;

public:

	/** Default constructor. */
	USplineAssetEditorSettings();
};