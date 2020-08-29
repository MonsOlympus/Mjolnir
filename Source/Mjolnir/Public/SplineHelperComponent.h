/*
	2016-2018 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
//#include "StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "SplineHelperComponent.generated.h"

class UStaticMesh;

UCLASS(ClassGroup = "SplineHelper", hideCategories = (Rendering, Replication, Input, Actor, Base, Collision, Shape, Transform, StaticMesh, Materials, Lighting, Physics, Mobile) )
class MJOLNIR_API USplineHelperComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	USplineHelperComponent();

public:

	/* Use this to override the default mesh for this Spline segment */
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* OverrideDefaultMesh;

	bool SerializeData(FString NewFileName, FString& SaveText);
	FString& DeSerializeData(FString ExistingFileName);
};