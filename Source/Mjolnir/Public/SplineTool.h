/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "SplineInterface.h"
#include "SplineTypes.h"
#include "SplineTool.generated.h"

class USceneComponent;
//class USplineComponent;
//class USplineMeshComponent;
class USplineBuilderComponent;
class UStaticMesh;
class UMaterialInterface;
class AActor;
class USplineAsset;
class UFloorAlignComponent;
//class ISplineInterface;
class USplineExtrusionAsset;

/*
"SplineTool":
{
	"Splines":[
	{
		"Name":"",
		"DefaultMeshPath":"",
		"DefaultMaterialPath":"",
		"DefaultAxis":"",
		"PreTransform":{	"LocX":"", "LocY":"", "LocZ":"",
								"RotX":"", "RotY":"", "RotZ":""
								"ScaleX":"", "ScaleY":"", "ScaleZ":""},
		"Nodes:"[{"BlueprintClassPath":""},{"BlueprintClassPath":""}]
	}]
}
	
	//,

*/

USTRUCT(BlueprintType)
struct FSplineBuilder
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshes;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Defaults")
	USplineExtrusionAsset* ExtrusionPreset = nullptr;

	UPROPERTY(EditAnywhere)
	ESplineType ExtrudeType = ESplineType::ST_Segment;

	UPROPERTY(EditAnywhere, Category = "Mesh")
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

	UPROPERTY(VisibleAnywhere)
	float SectionLength = 1.0f;

	/* Leave blank to use Default Mesh Collision */
	UPROPERTY(EditAnywhere)
	UStaticMesh* CollisionMesh = nullptr;

	UPROPERTY(EditAnywhere)
	float CollisionZOffset = 0.0f;

	/* Along the Splines Direction, U*/
	UPROPERTY(EditAnywhere)
	float TextureTilingX = 1.0f;

	/* Perpendicular to the Splines Direction, V*/
	UPROPERTY(EditAnywhere)
	float TextureTilingY = 1.0f;

	//UPROPERTY(EditDefaultsOnly)
	//UFloorAlignComponent* FloorAlignment = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Caps")
	UStaticMesh* StartCapMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Caps")
	UStaticMesh* EndCapMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TArray<FSplineNode> SplineSpawnNodes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Defaults")
	TArray <FSplineSpacer> SplineMeshSpacer;

	UPROPERTY(EditInstanceOnly, Category = "Taper")
	FVector2D TaperScale = FVector2D(1.0f, 1.0f);

	UPROPERTY(EditAnywhere)
	bool bProject;

	UPROPERTY(EditInstanceOnly)
	bool bUseRoll = false;

	UPROPERTY(EditInstanceOnly)
	bool bUseScale = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	float CullDistance = 100000.0f; //~1km

	FSplineBuilder()
	{

	}

	/*FSplineBuilder(USplineExtrusionAsset* Preset)
	{
		ExtrudeType = Preset->ExtrudeType;
		DefaultMesh = Preset->DefaultMesh;
	}*/

	FSplineBuilder(UStaticMesh* newDefaultMesh, UMaterialInterface* newDefaultMaterialOverride)
	{
		DefaultMesh = newDefaultMesh;
		DefaultMaterialOverride = newDefaultMaterialOverride;
	}
};

USTRUCT(BlueprintType)
struct FSplinePlacer
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Defaults")
	//USplineExtrusionAsset* ExtrusionPreset = nullptr;

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

	/* Direction this Mesh is facing by default (Should be X+) */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESplineMeshAxis::Type> MeshAxis = ESplineMeshAxis::X;

	UPROPERTY(EditAnywhere)
	float EdgeFactor;

	UPROPERTY(EditAnywhere)
	float ClumpFactor;

	UPROPERTY(EditAnywhere)
	bool bProject;

	UPROPERTY(EditAnywhere)
	bool bRotateToTangent;

	UPROPERTY(EditAnywhere)
	bool bUseStartPoint;
	
	UPROPERTY(EditAnywhere)
	bool bUseEndPoint;

	UPROPERTY(EditAnywhere)
	FTransform PreTransform;

	UPROPERTY(EditAnywhere)
	FRandomTransform RandomTransform;
	
	UPROPERTY(EditAnywhere)
	FVector InitialOffset;

	UPROPERTY(EditAnywhere)
	TArray<FName> Sockets;
};

UCLASS(HideCategories = ("Rendering", "Input", "HLOD", "Actor", "Replication", "LOD", "Cooking"), AutoCollapseCategories = ("Caps", "Stacks"))
class MJOLNIR_API ASplineTool : public AActor, public ISplineInterface
{
	GENERATED_BODY()

//protected:
	//UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Defaults")
	//FString FileName = "/Default";

public:
	// we don't want to have our components automatically destroyed by the Blueprint code
	//virtual void RerunConstructionScripts() override {}

	ASplineTool();

	UPROPERTY(VisibleDefaultsOnly)
	USplineBuilderComponent* FirstBuilder;

	UPROPERTY(VisibleDefaultsOnly)
	USceneComponent* Scene;

	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	//USplineAsset* SplinePreset;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta=(UnsafeDuringActorConstruction, ToolTip = "If you are experiencing issues with Mesh generation this may help."))
	void RegenerateSpline();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (UnsafeDuringActorConstruction, ToolTip = "Operates on all SplineBuilders, this will find the nearest 'ground' and snap the points to it"))
	void SnapAllToGround();

	//UFUNCTION(CallInEditor, BlueprintCallable, Category = "Defaults", meta = (UnsafeDuringActorConstruction, ToolTip = "Adds a new Spline Builder Component"))
	//void AddNewBuilder();

	//UFUNCTION(CallInEditor, BlueprintCallable, Category = "Builder", meta = (UnsafeDuringActorConstruction, ToolTip = "Adds an entry to the Spline Builders List"))
	//void AddNewEntryToBuilder();

	/* Default Extrusion for all newly created Splines */
	//UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Builder")
	//USplineExtrusionAsset* ExtrusionPreset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool DebugMode;

	/* Will use low detailed Materials for all Spline Meshes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Details")
	bool LowDetail;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginDestroy() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	//virtual void PostInitProperties() override;
};