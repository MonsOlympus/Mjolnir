/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "SplineTypes.h"
#include "SplineExtrusionAsset.generated.h"

UCLASS(ClassGroup = "Splines", ShowCategories = (Mobility), HideCategories = (Replication, Input, Actor, Base, Collision, Shape, StaticMesh, Materials, Lighting, Physics, Mobile, HLOD, "AssetUserData"), meta = (BlueprintSpawnableComponent))
class USplineExtrusionAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ESplineType ExtrudeType = ESplineType::ST_Segment;

	UPROPERTY(EditAnywhere)
	UStaticMesh* DefaultMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* DefaultMaterialOverride = nullptr;

	/* Tessellated Materials */
	UPROPERTY(EditAnywhere)
	UMaterialInterface* HighMaterialOverride = nullptr;

	/* Direction this Mesh is facing by default (Should be X+) */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESplineMeshAxis::Type> MeshAxis = ESplineMeshAxis::X;

	UPROPERTY(EditAnywhere)
	FTransform PreTransform;

	//TODO: Combine with PreTransform?
	UPROPERTY(EditAnywhere)
	FVector Offset = FVector(0.0f, 0.0f, 0.0f);

	/* Leave blank to use Default Mesh Collision */
	UPROPERTY(EditAnywhere)
	UStaticMesh* CollisionMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float CollisionZOffset;

	/* Along the Splines Direction, U*/
	UPROPERTY(EditDefaultsOnly)
	float TextureTilingX = 1.0f;

	/* Perpendicular to the Splines Direction, V*/
	UPROPERTY(EditDefaultsOnly)
	float TextureTilingY = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Caps")
	UStaticMesh* StartCapMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Caps")
	UStaticMesh* EndCapMesh;

//	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Caps")
//	FSplinePointCap SplineCaps;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TArray<FSplineNode> SplineSpawnNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TArray <FSplineSpacer> SplineMeshSpacer;

	UPROPERTY(EditInstanceOnly, Category = "Taper")
	FVector2D TaperScale = FVector2D(1.0f, 1.0f);

	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
};