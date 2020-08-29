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
#include "SplineTool.h"
#include "SplineBuilderComponent.generated.h"

class UStaticMesh;
class UInstancedStaticMeshComponent;

/**
 *	A spline builder component is a spline shape which is used for spline mesh auto-placement.
 */
UCLASS(ClassGroup = "Splines", ShowCategories = (Mobility), HideCategories = (Replication, Input, Actor, Base, Collision, Shape, StaticMesh, Materials, Lighting, Physics, Mobile, HLOD, "AssetUserData"), meta = (BlueprintSpawnableComponent))
class MJOLNIR_API USplineBuilderComponent : public USplineComponent
{
	GENERATED_BODY()

protected:
	USplineBuilderComponent(const FObjectInitializer& ObjectInitializer);

public:
	/* Builder */
	//UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Defaults")
	//	USplineExtrusionAsset* ExtrusionPreset = nullptr;

	/* Spline */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Spline")
	USplineAsset* SplinePreset;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Spline", meta = (UnsafeDuringActorConstruction, ToolTip = "Loads the selected Spline Asset"))
	void LoadSplineAsset();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Spline", meta = (UnsafeDuringActorConstruction, ToolTip = "Saves changes to the Spline Asset"))
	void SaveSplineAsset();

	UPROPERTY(EditAnywhere, Category = "_Builder")
	TArray<FSplineBuilder> Builders;

	UPROPERTY()
	TMap<uint8, UStaticMesh*> SegmentMeshHelper;

	UFUNCTION()
	void ClearSplineMeshes();

	UFUNCTION()
	void ExtrudeSplineMeshes();

	UFUNCTION()
	void ExtrudeSplineMesh(AActor* Master, uint32 BuilderIndex);

	/* Custom Collision Builder */
	UFUNCTION()
	bool BuildSplineMeshCollision(AActor* Master, USceneComponent* BuilderSceneComponent, FSplineConstruct Spline, UStaticMesh* CollisionMesh = nullptr, uint32 index = 0);

//	bool SerializeData(FString NewFileName, FString& SaveText);
//	FString& DeSerializeData(FString ExistingFileName);

public:
	/* Placer */
	UPROPERTY(EditAnywhere, Category = "_Placer")
	TArray<FSplinePlacer> Placers;

	UFUNCTION()
	UInstancedStaticMeshComponent* CreateInstancedMesh(AActor* Master, FVector Location, FRotator Rotation = FRotator(0, 0, 0),  UStaticMesh* Mesh = nullptr);

	UFUNCTION()
	void ClearSplinePlacers();

	UFUNCTION()
	void PlaceMeshesAlongSpline();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//virtual void PostEditUndo() override;
	virtual void PreEditUndo() override;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (UnsafeDuringActorConstruction, ToolTip = ""))
	void GenerateBasicSpline();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (UnsafeDuringActorConstruction, ToolTip = "This will find the nearest 'ground' and snap all the points to it"))
	void SnapToGround();

	UPROPERTY(EditAnywhere, Category = "Alignment")
	float FloorTraceDistance = 3500;

	UPROPERTY(EditAnywhere, Category = "Alignment")
	float FloorOffset = 12;

	UPROPERTY(EditAnywhere, Category = "Alignment")
	bool bIgnoreOtherSplines;

	UPROPERTY(EditAnywhere, Category = "Alignment")
	bool bDebugFloorSnap;

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Debug", meta = (UnsafeDuringActorConstruction, ToolTip = "Will remove all Debug Lines"))
	void ClearDebugLines();


	//void RegenerateSpline();

#endif //WITH_EDITOR

	//void UpdateSpline() override;
};