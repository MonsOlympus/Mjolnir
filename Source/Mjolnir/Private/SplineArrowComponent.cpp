/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#include "SplineArrowComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

class UStaticMesh;
class UMaterialInterface;
class UInstancedStaticMeshComponent;

USplineArrowComponent::USplineArrowComponent()
{
	static ConstructorHelpers::FObjectFinder<UMaterial> foundMaterial(TEXT("Material'/Mjolnir/SplineToolHelperMat.SplineToolHelperMat'"));
	if (foundMaterial.Object != nullptr)
	{
		DefaultMaterial = foundMaterial.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> foundStaticMeshA(TEXT("StaticMesh'/Mjolnir/SplineTrace.SplineTrace'"));
	if (foundStaticMeshA.Object != nullptr)
	{
		DefaultArrowStem = foundStaticMeshA.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> foundStaticMeshB(TEXT("StaticMesh'/Mjolnir/SplineTraceArrow.SplineTraceArrow'"));
	if (foundStaticMeshB.Object != nullptr)
	{
		DefaultArrowTip = foundStaticMeshB.Object;
	}
}

void USplineArrowComponent::AddArrow(FVector Start, FVector End, FLinearColor Color)
{
	AActor* Master = GetOwner();
	if (Master == nullptr)
		return;

	USplineComponent* Spline = NewObject <USplineComponent>(Master);
	//FSplineTrace tempArrow(Master, Spline,  Start, End, Color);

	FVector locStart, locEnd;

	Spline->AddSplinePoint(Start, ESplineCoordinateSpace::World, false);
	Spline->AddSplinePoint(End, ESplineCoordinateSpace::World, true);

	//for (int32 Index = 0; Index < Spline->GetNumberOfSplinePoints() - 1; Index++)
	//{
		auto *SplineMeshHelper = NewObject <USplineMeshComponent>(Master);
		if (SplineMeshHelper == nullptr)
			return;

		SplineMeshHelper->CreationMethod = EComponentCreationMethod::Native;
		//SplineMeshHelper->SetMobility(EComponentMobility::Type::Moveable);
		SplineMeshHelper->RegisterComponent();
		SplineMeshHelper->SetStaticMesh(DefaultArrowStem);
		//SplineMeshHelper->SetMaterial(0, DefaultMaterial);

		//locStart = Spline->GetWorldLocationAtSplinePoint(Index);
		//locEnd = Spline->GetWorldLocationAtSplinePoint(Index + 1);

		SplineMeshHelper->SetStartPosition(Start);
		SplineMeshHelper->SetEndPosition(End);

		auto *Mesh = NewObject <UStaticMeshComponent>(Master);
		if (Mesh == nullptr)
			return;

		Mesh->SetWorldLocation(locEnd);

		FSplineTrace tempArrow(Spline, Start, End, Color);
		Arrows.Add(tempArrow);
	//}
}