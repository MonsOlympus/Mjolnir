/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "Internationalization/Text.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Components/SplineComponent.h"

#include "SplineAsset.generated.h"

/*
"Level"={
	"Name"="",
	"Splines"=[
		"Actor"=
		{
			"Name"="ActorName",
			"Spline"=
			{
				"Name"="",
				"DefaultMeshPath"="",
				"DefaultMaterialOverridePath"="",
				"HighMaterialOverridePath"="",
				"DefaultAxis"="",
				"PreTransform"="",

				"SplineHelpers"=[
					"SplineHelper"=
					{
						"SegmentIndex"="",
						"DefaultMeshPath"="",
						"DefaultMaterialOverridePath"="",
						"HighMaterialOverridePath"="",
						"DefaultAxis"="",
						"PreTransform"=""
					},
					"SplineHelper"={}
				]
			}
		},
		"Actor"=
		{
			"Name"="",
			"Spline"={}
		}
	]
}
*/

/**
 * Implements an asset that can be used to store arbitrary text, such as notes
 * or documentation.
 */
UCLASS(BlueprintType, hidecategories = (Object))
class MJOLNIR_API USplineAsset	: public UObject
{
	GENERATED_BODY()

public:

	/** Holds the stored text. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText SplineJSON;

	UFUNCTION()
	void LoadSpline(FSplineCurves& SplineCurves);

	UFUNCTION()
	void SaveSpline(FSplineCurves SplineCurves);
};