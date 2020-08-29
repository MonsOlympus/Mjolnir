/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "SplineTypes.h"
#include "SplinePlacementAsset.generated.h"

class UStaticMesh;

UCLASS(ClassGroup = "Splines", ShowCategories = (Mobility), HideCategories = (Replication, Input, Actor, Base, Collision, Shape, StaticMesh, Materials, Lighting, Physics, Mobile, HLOD, "AssetUserData"), meta = (BlueprintSpawnableComponent))
class USplinePlacementAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<UInstancedStaticMeshComponent*> InstancedMeshes;

	UPROPERTY(EditAnywhere)
	EPlaceType PlacementType = EPlaceType::PT_Spline;

	UPROPERTY(EditAnywhere, Category = "_Meshes")
	int32 Subdivisions = 0;

	UPROPERTY(EditAnywhere, Category = "_Meshes")
	float Gap = 0.0f;

	UPROPERTY(EditAnywhere, Category = "_Meshes")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "_Meshes")
	UBlueprint* Blueprint;

	UPROPERTY(EditAnywhere)
	FTransform PreTransform;

	UPROPERTY(EditAnywhere)
	bool bRotateToTangent;

	UPROPERTY(EditAnywhere)
	bool bUseStartPoint;

	UPROPERTY(EditAnywhere)
	bool bUseEndPoint;

	/* Transform Variation */
	UPROPERTY(EditAnywhere)
	FRandomTransform RandomTransform;

	UPROPERTY(EditAnywhere)
	FVector InitialOffset;

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};