/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "SplineArrowComponent.generated.h"

class USceneComponent;

USTRUCT(BlueprintType)
struct FSplineTrace
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditInstanceOnly)
	USplineComponent* Spline;

	//UPROPERTY(EditInstanceOnly)
	//UStaticMeshComponent* Mesh;

//	UPROPERTY(EditInstanceOnly)
//	FVector Start;

//	UPROPERTY(EditInstanceOnly)
//	FVector End;

	UPROPERTY(EditInstanceOnly)
	FLinearColor Color;

public:
	FSplineTrace() {}

	FSplineTrace(USplineComponent* newSpline, FVector newStart, FVector newEnd, FLinearColor newColor)
	{
		//Start = newStart;
		//End = newEnd;
		Color = newColor;

		/*auto *Spline = NewObject <USplineMeshComponent>(Owner);
		if (Spline == nullptr)
			return;*/

		//newSpline.AddSplinePoint(newStart, ESplineCoordinateSpace::World, false);
		//newSpline.AddSplinePoint(newEnd, ESplineCoordinateSpace::World, true);
		Spline = newSpline;
	}
};

/**
 *	
 */
UCLASS(ClassGroup = "Splines", ShowCategories = (Mobility), HideCategories = (Replication, Input, Actor, Base, Collision, Shape, StaticMesh, Materials, Lighting, Physics, Mobile, HLOD, "AssetUserData"), meta = (BlueprintSpawnableComponent))
class MJOLNIR_API USplineArrowComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	TArray<FSplineTrace> Arrows;

public:
	USplineArrowComponent();

	UPROPERTY()
	UMaterial* DefaultMaterial;

	UPROPERTY()
	UStaticMesh* DefaultArrowStem;

	UPROPERTY()
	UStaticMesh* DefaultArrowTip;

	void AddArrow(FVector Start, FVector End, FLinearColor Color);
};