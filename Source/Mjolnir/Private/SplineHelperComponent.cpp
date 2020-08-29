/*
	2010-2018 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#include "SplineHelperComponent.h"

class IFileHandle;
class UStaticMesh;
class UMaterialInterface;
class UFileToolsBlueprintLibrary;

USplineHelperComponent::USplineHelperComponent()
{
	/*static UConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshObj(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	SetStaticMesh(StaticMeshObj.Object);

	static UConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObj(TEXT("/Game/SplineToolHelperMat.SplineToolHelperMat"));
	SetMaterial(0, MaterialObj.Object);*/
}
