/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#include "SplineBuilderComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "SplineExtrusionAsset.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine.h"

class IFileHandle;
class UStaticMesh;
class UMaterialInterface;
class UInstancedStaticMeshComponent;
class UKismetMathLibrary;

USplineBuilderComponent::USplineBuilderComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITORONLY_DATA
	EditorUnselectedSplineSegmentColor = FLinearColor(1.0f, 1.0f, 1.0f);
	EditorSelectedSplineSegmentColor = FLinearColor(0.5f, 1.0f, 0.5f);
	bAllowDiscontinuousSpline = false;
	bShouldVisualizeScale = true;
	ScaleVisualizationWidth = 5.0f;
#endif

	/*SplineCurves.Position.Points.Reset(10);
	SplineCurves.Rotation.Points.Reset(10);
	SplineCurves.Scale.Points.Reset(10);

	SplineCurves.Position.Points.Emplace(0.0f, FVector(0, 0, 0), FVector::ZeroVector, FVector::ZeroVector, CIM_CurveAuto);
	SplineCurves.Rotation.Points.Emplace(0.0f, FQuat::Identity, FQuat::Identity, FQuat::Identity, CIM_CurveAuto);
	SplineCurves.Scale.Points.Emplace(0.0f, FVector(1.0f), FVector::ZeroVector, FVector::ZeroVector, CIM_CurveAuto);

	SplineCurves.Position.Points.Emplace(0.5f, FVector(100, 0, 0), FVector::ZeroVector, FVector::ZeroVector, CIM_CurveAuto);
	SplineCurves.Rotation.Points.Emplace(0.5f, FQuat::Identity, FQuat::Identity, FQuat::Identity, CIM_CurveAuto);
	SplineCurves.Scale.Points.Emplace(0.5f, FVector(1.0f), FVector::ZeroVector, FVector::ZeroVector, CIM_CurveAuto);

	SplineCurves.Position.Points.Emplace(1.0f, FVector(200, 0, 0), FVector::ZeroVector, FVector::ZeroVector, CIM_CurveAuto);
	SplineCurves.Rotation.Points.Emplace(1.0f, FQuat::Identity, FQuat::Identity, FQuat::Identity, CIM_CurveAuto);
	SplineCurves.Scale.Points.Emplace(1.0f, FVector(1.0f), FVector::ZeroVector, FVector::ZeroVector, CIM_CurveAuto);*/

	UpdateSpline();

	// Set these deprecated values up so that old assets with default values load correctly (and are subsequently upgraded during Serialize)
	SplineInfo_DEPRECATED = SplineCurves.Position;
	SplineRotInfo_DEPRECATED = SplineCurves.Rotation;
	SplineScaleInfo_DEPRECATED = SplineCurves.Scale;
	SplineReparamTable_DEPRECATED = SplineCurves.ReparamTable;
	///==========================

	bInputSplinePointsToConstructionScript = true;
	//bInputSplinePointsToConstructionScript = false;
}


#if WITH_EDITOR
/*void USplineBuilderComponent::RegenerateSpline()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Regenerate"));

	TArray<USceneComponent*> SubScenes;
	GetChildrenComponents(true, SubScenes);
	for (USceneComponent* SubScene : SubScenes)
	{
		TArray<USplineMeshComponent*> SplineMeshes;
		SubScene->GetChildrenComponents(false, &SplineMeshes);
		for (USplineMeshComponent* SplineMesh : SplineMeshes)
		{
			SplineMesh->DestroyComponent();
		}

		TArray<UInstancedStaticMeshComponent*> PlacedMeshes;
		SubScene->GetChildrenComponents(false, PlacedMeshes);
		for (UInstancedStaticMeshComponent* PlacedMesh : PlacedMeshes)
		{
			PlacedMesh->DestroyComponent();
		}
	}

	ExtrudeSplineMeshes();
	PlaceMeshesAlongSpline();

	AActor* MasterOwner = GetOwner();
	if (GetWorld() != nullptr && MasterOwner != nullptr)
	{
		MasterOwner->RegisterAllComponents();
	}
}*/

void USplineBuilderComponent::GenerateBasicSpline()
{
/*	SplineCurves.Position.Points.Reset();
	SplineCurves.Rotation.Points.Reset();
	SplineCurves.Scale.Points.Reset();*/

	ClearSplinePoints();

	AddPoint(FSplinePoint(0.0f, FVector(0, 0, 0)));
	//AddPoint(FSplinePoint(0.5f, FVector(150, 0, 0)));
	AddPoint(FSplinePoint(1.0f, FVector(300, 0, 0)));
}

void USplineBuilderComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Changed Property"));

	AActor* MasterOwner = GetOwner();

	if (PropertyChangedEvent.Property != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Edited Property %s"), *PropertyName.ToString());

		static const FName SplineCurvesName = GET_MEMBER_NAME_CHECKED(USplineComponent, SplineCurves);
	
		static const FName BuildersName = GET_MEMBER_NAME_CHECKED(USplineBuilderComponent, Builders);
		static const FName SplinePresetName = GET_MEMBER_NAME_CHECKED(USplineBuilderComponent, SplinePreset);
		static const FName ExtrusionPresetName = GET_MEMBER_NAME_CHECKED(FSplineBuilder, ExtrusionPreset);

			
		const FName PropertyName(PropertyChangedEvent.Property->GetFName());

		//ASplineTool* Master = Cast<ASplineTool*>(GetOwner());

		//TODO: This doesnt work correctly, makes any modification to the Spline dissappear the mesh
		if (PropertyName == SplineCurvesName)
		{
			//RegenerateSpline();
			//ClearSplinePlacers();
			//ClearSplineMeshes();
			//UE_LOG(LogTemp, Error, TEXT("MultiSpline: Spline Updated"));
		}

		if (PropertyName == BuildersName ||	/*PropertyName == SplinePresetName ||*/ PropertyName == ExtrusionPresetName)
		{
			//ClearSplineMeshes();
			//RegenerateSpline();
		}

		/*if (PropertyName == ExtrusionPresetName){UE_LOG(LogTemp, Error, TEXT("MultiSpline: Extrusion Edited"));}*/

		static const FName SubdivisionsPresetName = GET_MEMBER_NAME_CHECKED(FSplinePlacer, Subdivisions);
		static const FName PlacerMeshPresetName = GET_MEMBER_NAME_CHECKED(FSplinePlacer, Mesh);
		static const FName PlacementPresetName = GET_MEMBER_NAME_CHECKED(FSplinePlacer, PlacementType);
		static const FName RotateToTangentPresetName = GET_MEMBER_NAME_CHECKED(FSplinePlacer, bRotateToTangent);
		static const FName StartPointPresetName = GET_MEMBER_NAME_CHECKED(FSplinePlacer, bUseStartPoint);
		static const FName EndPointPresetName = GET_MEMBER_NAME_CHECKED(FSplinePlacer, bUseEndPoint);

//		static const FName PlacerPresetName = GET_MEMBER_NAME_CHECKED(ASplineTool, FSplinePlacer);
		
		static const FName PlacersPresetName = GET_MEMBER_NAME_CHECKED(USplineBuilderComponent, Placers);


		if (PropertyName == PlacersPresetName || PropertyName == SubdivisionsPresetName || PropertyName == PlacerMeshPresetName || PropertyName == PlacementPresetName
			|| PropertyName == RotateToTangentPresetName || PropertyName == StartPointPresetName || PropertyName == EndPointPresetName)
		{
			//ClearSplinePlacers();
			//RegenerateSpline();
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}

/*void USplineBuilderComponent::PostEditUndo()
{
	Super::PostEditUndo();
	UE_LOG(LogTemp, Error, TEXT("MultiSpline: Post Undo"));
	//ClearSplineMeshes();
}*/

void USplineBuilderComponent::PreEditUndo()
{
	Super::PreEditUndo();
	//UE_LOG(LogTemp, Error, TEXT("MultiSpline: Pre Undo"));
	//ClearSplineMeshes();
	//ClearSplinePlacers();
	//RegenerateSpline();
}
#endif //WITH_EDITOR

/*bool USplineBuilderComponent::CanEditChange(const UProperty* InProperty) const
{
	bool bCanEdit = Super::CanEditChange(InProperty);

	FName PropertyName = InProperty ? InProperty->GetFName() : NAME_None;

	if (GET_MEMBER_NAME_CHECKED(UUSDImportOptions, MeshImportType) == PropertyName ||
		GET_MEMBER_NAME_CHECKED(UUSDImportOptions, bApplyWorldTransformToGeometry) == PropertyName ||
		GET_MEMBER_NAME_CHECKED(UUSDImportOptions, bGenerateUniquePathPerUSDPrim) == PropertyName)
	{
		bCanEdit &= bImportMeshes;
	}

	return bCanEdit;
}*/

/*void USplineBuilderComponent::UpdateSpline()
{
	//SplineCurves.UpdateSpline(bClosedLoop, bStationaryEndpoints, ReparamStepsPerSegment, bLoopPositionOverride, LoopPosition, GetComponentTransform().GetScale3D());
#if WITH_EDITOR
	ClearSplineMeshes();
#endif //WITH_EDITOR
	Super::UpdateSpline();
}*/

void USplineBuilderComponent::LoadSplineAsset()
{
	if (SplinePreset != nullptr)
	{
		ClearSplinePoints(false);
		SplinePreset->LoadSpline(SplineCurves);
		UpdateSpline();
		//ClearSplineMeshes();
		//ExtrudeSplineMeshes();
		//RegenerateSpline();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MultiSpline: Cannot load, no Spline Preset selected!"));
	}
}

void USplineBuilderComponent::SaveSplineAsset()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Save Asset"));

	if (SplinePreset != nullptr)
	{
		SplinePreset->SaveSpline(SplineCurves);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MultiSpline: Cannot save, no Spline Preset selected!"));
	}
}

void USplineBuilderComponent::PlaceMeshesAlongSpline()
{
	AActor* MasterOwner = GetOwner();

	/*float temp = Subdivisions / GetNumberOfSplinePoints();
	float SectionLength = (GetSplineLength() / GetNumberOfSplinePoints()) * temp;
	int32 numPoints;
	if (temp == 0)	{numPoints = 0;}
	else{numPoints = FMath::TruncToInt(GetSplineLength() / SectionLength);}*/

	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Meshes %f / %f"), GetSplineLength(), SectionLength);
	//int32 numPoints = FMath::TruncToInt(GetSplineLength() / SectionLength);
	//FMath::TruncToInt((Subdivisions / GetNumberOfSplinePoints())*GetNumberOfSplinePoints());
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Meshes %d"), numPoints);

	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	//Random
	//int32 Seed = 0;
	//FRandomStream* RandomStream;
	//RandomStream->Initialize(Seed);
	
	if (MasterOwner != nullptr)
	{
		float SectionLength = 0.0f;
		int32 numPoints = 0;
		int32 Index = 0;

		for (int32 PlacerIndex = 0; PlacerIndex < Placers.Num(); PlacerIndex++)
		{
			FString PlacerStringName = (TEXT("SplinePlacer_")) + FString::FromInt(PlacerIndex);
			FName PlacerName = *PlacerStringName;

			auto *PlacerScene = NewObject <USceneComponent>(MasterOwner, PlacerName);
			if (PlacerScene == nullptr)
				return;

			PlacerScene->AttachToComponent(this, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			FSplinePlacer Placer = Placers[PlacerIndex];

			Placer.InstancedMeshes.Empty();
			switch (Placer.PlacementType)
			{
			case EPlaceType::PT_Spline:
				SectionLength = GetSplineLength() / Placer.Subdivisions -1 + Placer.bUseStartPoint + Placer.bUseEndPoint;
				//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Meshes %f / %f"), GetSplineLength(), SectionLength);
				for (Index = 0; Index < Placer.Subdivisions; Index++)
				{
					if (!Placer.bUseStartPoint && Index != 0 || !Placer.bUseEndPoint && Index != Placer.Subdivisions)
					{
						Rotation = Placer.PreTransform.Rotator();
						FVector Location = GetLocationAtDistanceAlongSpline(SectionLength * Index, ESplineCoordinateSpace::Local/*World*/);
						
						if (Placer.bRotateToTangent)
						{
							Rotation = GetRotationAtDistanceAlongSpline(SectionLength * Index, ESplineCoordinateSpace::Local/*World*/);
							Rotation = UKismetMathLibrary::ComposeRotators(Rotation, Placer.PreTransform.Rotator());
						}

						UInstancedStaticMeshComponent* Instance = CreateInstancedMesh(MasterOwner, Location, Rotation, Placer.Mesh);
						if (Instance != nullptr)
						{
							Placer.InstancedMeshes.Add(Instance);
							Instance->AttachToComponent(PlacerScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
						}
					}
				}
				break;

			case EPlaceType::PT_Segment:
				//numPoints = FMath::TruncToInt(Placer.Subdivisions * GetNumberOfSplinePoints());
				for (Index = 0; Index < GetNumberOfSplinePoints()-1; Index++)
				{
					SectionLength = GetDistanceAlongSplineAtSplinePoint(Index+1) - GetDistanceAlongSplineAtSplinePoint(Index);
					//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SectionLength %f "), SectionLength);
					for (int32 DivisionIndex = 0; DivisionIndex < Placer.Subdivisions; DivisionIndex++)
					{
						float tempLength = GetDistanceAlongSplineAtSplinePoint(Index) + (SectionLength / Placer.Subdivisions)*DivisionIndex;
						FVector Location = GetLocationAtDistanceAlongSpline(tempLength, ESplineCoordinateSpace::Local/*World*/);
						Rotation = Placer.PreTransform.Rotator();

						if (Placer.bRotateToTangent)
						{
							Rotation = GetRotationAtDistanceAlongSpline(SectionLength * Index, ESplineCoordinateSpace::Local/*World*/);
							Rotation = UKismetMathLibrary::ComposeRotators(Rotation, Placer.PreTransform.Rotator());
						}

						UInstancedStaticMeshComponent* Instance = CreateInstancedMesh(MasterOwner, Location, Rotation, Placer.Mesh);
						if (Instance != nullptr)
						{
							Placer.InstancedMeshes.Add(Instance);
							Instance->AttachToComponent(PlacerScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
						}
					}
				}
				break;

			case EPlaceType::PT_Gap:
				//TODO: Get mesh width so they dont clip, maximum points
				numPoints = FMath::TruncToInt(GetSplineLength() / Placer.Gap);
				for (Index = 0; Index <= numPoints; Index++)
				{
					if (!Placer.bUseStartPoint && Index == 0 || !Placer.bUseEndPoint && Index == numPoints) {}
					else
					{
						FVector Location = GetLocationAtDistanceAlongSpline(Placer.Gap * Index, ESplineCoordinateSpace::Local/*World*/);
						Rotation = Placer.PreTransform.Rotator();

						if (Placer.bRotateToTangent)
						{
							Rotation = GetRotationAtDistanceAlongSpline(SectionLength * Index, ESplineCoordinateSpace::Local/*World*/);
							Rotation = UKismetMathLibrary::ComposeRotators(Rotation, Placer.PreTransform.Rotator());
						}

						UInstancedStaticMeshComponent* Instance = CreateInstancedMesh(MasterOwner, Location, Rotation, Placer.Mesh);
						if (Instance != nullptr)
						{
							Placer.InstancedMeshes.Add(Instance);
							Instance->AttachToComponent(PlacerScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
						}
					}
				}
				break;

			case EPlaceType::PT_Scatter:
				//numPoints = GetNumberOfSplinePoints();
				//for (Index = 0; Index < numPoints; Index++)
				//{
				for (Index = 0; Index < Placer.Subdivisions; Index++)
				{
					FVector VectorInRange = FMath::RandPointInBox(FBox(Placer.RandomTransform.LocationMin, Placer.RandomTransform.LocationMax));
					//UE_LOG(LogTemp, Warning, TEXT("RandPointInBox %s"), *VectorInRange.ToString());
					float Distance = FMath::FRandRange(0.0f, GetSplineLength());
					FVector Location = GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
					UE_LOG(LogTemp, Warning, TEXT("Location %s"), *Location.ToString());
					//Location = VectorInRange * Location;
					//Location.X = VectorInRange.X * Location.X;
					Location.Y = VectorInRange.Y;
					Location.Z = VectorInRange.Z;
					//UE_LOG(LogTemp, Warning, TEXT("Location %s"), *Location.ToString());

					Rotation = Placer.PreTransform.Rotator();

					FRotator RotationInRange;
					RotationInRange.Pitch = FMath::FRandRange(Placer.RandomTransform.RotationMin.Pitch, Placer.RandomTransform.RotationMax.Pitch);
					RotationInRange.Roll = FMath::FRandRange(Placer.RandomTransform.RotationMin.Roll, Placer.RandomTransform.RotationMax.Roll);
					RotationInRange.Yaw = FMath::FRandRange(Placer.RandomTransform.RotationMin.Yaw, Placer.RandomTransform.RotationMax.Yaw);

					Rotation = UKismetMathLibrary::ComposeRotators(Rotation, RotationInRange);

					UInstancedStaticMeshComponent* Instance = CreateInstancedMesh(MasterOwner, Location, Rotation, Placer.Mesh);
					if (Instance != nullptr)
					{
						Placer.InstancedMeshes.Add(Instance);
						Instance->AttachToComponent(PlacerScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
					}
				}
				break;

			case EPlaceType::PT_Socket:
				for (FName Socket : Placer.Sockets)
				{
					//virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;

				}
				break;

			case EPlaceType::PT_SmartScatter:

				break;
			}
		}
	}
}

UInstancedStaticMeshComponent* USplineBuilderComponent::CreateInstancedMesh(AActor* Master, FVector Location, FRotator Rotation, UStaticMesh* Mesh)
{
	if (Master == nullptr)
		return nullptr;

	auto *InstanceMesh = NewObject <UInstancedStaticMeshComponent>(Master);
	if (InstanceMesh == nullptr)
		return nullptr;

	InstanceMesh->CreationMethod = EComponentCreationMethod::Instance;
	InstanceMesh->SetMobility(EComponentMobility::Type::Stationary);
	InstanceMesh->AddInstance(FTransform(Rotation, Location, FVector(1.0f, 1.0f, 1.0f)));
	InstanceMesh->RegisterComponent();
	InstanceMesh->SetStaticMesh(Mesh);
	return InstanceMesh;
}

void USplineBuilderComponent::ClearSplinePlacers()
{
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Remove Meshes %d"), Builders[SplineBuilderIndex].SplineMeshes.Num());
/*	for (auto Placer : Placers)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Placer - %d"), Placer.InstancedMeshes.Num());
		for (int32 PlacerIndex = Placer.InstancedMeshes.Num() - 1; PlacerIndex > -1; PlacerIndex--)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Builder - %d"), Placer.InstancedMeshes[PlacerIndex]);
			Placer.InstancedMeshes[PlacerIndex]->DestroyComponent();
			Placer.InstancedMeshes.RemoveAt(PlacerIndex);
		}
	}*/

	for (int32 PlacerIndex = 0; PlacerIndex < Placers.Num(); PlacerIndex++)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Placer - %d"), Placers[PlacerIndex].InstancedMeshes.Num());
		for (int32 Index = Placers[PlacerIndex].InstancedMeshes.Num() - 1; Index > -1; Index--)
		{
			Placers[PlacerIndex].InstancedMeshes[Index]->DestroyComponent();
			Placers[PlacerIndex].InstancedMeshes.RemoveAt(Index);
		}
	}
}

void USplineBuilderComponent::ExtrudeSplineMeshes()
{
	AActor* MasterOwner = GetOwner();
	for (int32 SplineBuilderIndex = 0; SplineBuilderIndex < Builders.Num(); SplineBuilderIndex++)
	{
		/* Early Out if we have no Owner */
		if (MasterOwner != nullptr)
		{
			ExtrudeSplineMesh(MasterOwner, SplineBuilderIndex);
		}
		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SplineMeshes"));
	}
}

void USplineBuilderComponent::ExtrudeSplineMesh(AActor* Master, uint32 BuilderIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: ExtrudeSplineMesh"));

	FSplineBuilder* Builder = &Builders[BuilderIndex];
	//const FVector NewScale = Builder->PreTransform.GetScale3D();
	
	//FSplineBuilder Builder = Builders[BuilderIndex];

	if (Builder->ExtrusionPreset != nullptr)
	{
		Builder->DefaultMesh = Builder->ExtrusionPreset->DefaultMesh;
		Builder->Offset = Builder->ExtrusionPreset->Offset;
		//Builder->PreTransform = Builder->ExtrusionPreset->PreTransform;

		if (Builder->ExtrusionPreset->DefaultMaterialOverride != nullptr)
		{
			Builder->DefaultMaterialOverride = Builder->ExtrusionPreset->DefaultMaterialOverride;
			Builder->TextureTilingX = Builder->ExtrusionPreset->TextureTilingX;
			Builder->TextureTilingY = Builder->ExtrusionPreset->TextureTilingY;
		}
	}

	if (Builder->DefaultMesh == nullptr)
		return;

	const FVector NewScale = Builder->PreTransform.GetScale3D();

	//Adjust SectionLength based on facing direction for CurrentMeshAxis
	switch (Builder->MeshAxis)
	{
	case ESplineMeshAxis::X:
		Builder->SectionLength = FVector(Builder->DefaultMesh->GetBounds().GetBox().GetExtent()).X;
		ScaleVisualizationWidth = NewScale.X * FVector(Builder->DefaultMesh->GetBounds().GetBox().GetExtent()).X;
		break;

	case ESplineMeshAxis::Y:
		Builder->SectionLength = FVector(Builder->DefaultMesh->GetBounds().GetBox().GetExtent()).Y;
		ScaleVisualizationWidth = NewScale.Y * FVector(Builder->DefaultMesh->GetBounds().GetBox().GetExtent()).Y;
		break;

	case ESplineMeshAxis::Z:
		Builder->SectionLength = FVector(Builder->DefaultMesh->GetBounds().GetBox().GetExtent()).Z;
		break;
	}

	Builder->SectionLength *= NewScale.Z;
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SectionLength: %f"), Builder.SectionLength);

	FVector locStart, locEnd, tanStart, tanEnd;
	FSplineConstruct Spline;
	int32 numPoints = GetNumberOfSplinePoints();
	FVector2D PreviousScale;
	float distance = 0.0f;
	float tempLength = 0.0f;
	float lengthRemainder = 0.0f;

	float StartCapExtent = 0.0f;
	float EndCapExtent = 0.0f;

	if (numPoints <= 0)
		return;

	FString BuilderStringName = (TEXT("SplineBuilder_")) + FString::FromInt(BuilderIndex);
	FName BuilderName = *BuilderStringName;

	auto *BuilderScene = NewObject <USceneComponent>(Master, BuilderName);
	if (BuilderScene == nullptr)
		return;

	BuilderScene->AttachToComponent(this, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

	//FVector GetScaleAtSplineInputKey(float InKey) const;
	//FTransform GetTransformAtSplinePoint(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseScale = false) const;

	//FVector GetScaleAtDistanceAlongSpline(float Distance) const;
	//	FTransform GetTransformAtDistanceAlongSpline(float Distance, ESplineCoordinateSpace::Type CoordinateSpace, bool bUseScale = false) const;


	switch (Builder->ExtrudeType)
	{
	case ESplineType::ST_MeshScale:
		numPoints = FMath::TruncToInt(GetSplineLength() / Builder->SectionLength + Builder->Offset.Z);

		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Mesh Scale - %d / %d"), numPoints, GetNumberOfSplinePoints());

		//for (int32 Index = 0; Index < numPoints - 1; Index++)
		for (int32 Index = 0; Index < numPoints; Index++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SplineMeshes %i"), idx);

			auto *SplineMeshHelper = NewObject <USplineMeshComponent>(Master);
			if (SplineMeshHelper == nullptr)
				return;

			SplineMeshHelper->CreationMethod = EComponentCreationMethod::Instance;
			SplineMeshHelper->SetMobility(EComponentMobility::Type::Stationary);//Type::Movable
			SplineMeshHelper->RegisterComponent();
			Builder->SplineMeshes.Add(SplineMeshHelper);

			SplineMeshHelper->SetStaticMesh(Builder->DefaultMesh);

			//if (NewScale.X != 1.0f && NewScale.Y != 1.0f)
			//{
			//SplineMeshHelper->SetStartScale(FVector2D(NewScale.X, NewScale.Y), true);
			//SplineMeshHelper->SetEndScale(FVector2D(NewScale.X, NewScale.Y), true);
			//}
			/*if (Builder.TaperScale.X != 1.0f && Builder.TaperScale.Y != 1.0f)
			{
				float idxPercent = float(idx + 1)*float(GetNumberOfSplinePoints()) / 100.0f;
				UE_LOG(LogTemp, Warning, TEXT("MultiSpline: %f"), idxPercent);
				if (idx == 0)
				{
					SplineMeshHelper->SetStartScale(FVector2D(NewScale.X, NewScale.Y), true);
				}
				else
				{
					SplineMeshHelper->SetStartScale(PreviousScale, true);
				}
				PreviousScale.X = UKismetMathLibrary::MapRangeClamped(idxPercent, 0.0f, 1.0f, NewScale.X, Builder.TaperScale.X);
				PreviousScale.Y = UKismetMathLibrary::MapRangeClamped(idxPercent, 0.0f, 1.0f, NewScale.Y, Builder.TaperScale.Y);
				SplineMeshHelper->SetEndScale(PreviousScale, true);
			}*/

			/** TODO: Support Material Arrays */
			if (Builder->DefaultMaterialOverride != nullptr)
			{
				SplineMeshHelper->SetMaterial(0, Builder->DefaultMaterialOverride);

				UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(SplineMeshHelper->GetMaterial(0), SplineMeshHelper);
				if (MID != nullptr)
				{
					MID->SetScalarParameterValue(FName(TEXT("U Scale Multiplier")), Builder->TextureTilingX);
					MID->SetScalarParameterValue(FName(TEXT("V Scale Multiplier")), Builder->TextureTilingY);
					SplineMeshHelper->SetMaterial(0, MID);
				}
			}

			distance = Builder->Offset.Z + Builder->SectionLength * Index;
			locStart = GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			locStart.Z += Builder->Offset.X;
			locStart.Y += Builder->Offset.Y;

			tanStart = GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			tanStart = tanStart.GetClampedToSize(0, Builder->SectionLength);

			float rollStart = GetRollAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			//FQuat GetQuaternionAtDistanceAlongSpline(float Distance, ESplineCoordinateSpace::Type CoordinateSpace) const;

			FVector scaleStart = GetScaleAtDistanceAlongSpline(distance);

			distance = Builder->Offset.Z + Builder->SectionLength * (Index + 1);
			locEnd = GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			locEnd.Z += Builder->Offset.X;
			locEnd.Y += Builder->Offset.Y;

			tanEnd = GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			tanEnd = tanEnd.GetClampedToSize(0, Builder->SectionLength);

			float rollEnd = GetRollAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);

			FVector scaleEnd = GetScaleAtDistanceAlongSpline(distance);

			SplineMeshHelper->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, true);
			Spline = FSplineConstruct(locStart, locEnd, tanStart, tanEnd, NewScale);

			//TODO: FIXME
			if (Builder->bUseRoll)
			{
				SplineMeshHelper->SetStartRoll(rollStart);
				SplineMeshHelper->SetEndRoll(rollEnd);
			}

			//TODO: 
			if (Builder->bUseScale)
			{
				SplineMeshHelper->SetStartScale(FVector2D(scaleStart.X, scaleStart.Y));
				SplineMeshHelper->SetEndScale(FVector2D(scaleEnd.X, scaleEnd.Y));
			}

			SplineMeshHelper->AttachToComponent(/*this*/ BuilderScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			/* Standard Collision if no Custom Mesh found */
			if (!BuildSplineMeshCollision(Master, BuilderScene, Spline, Builder->CollisionMesh, Index))
			{
				SplineMeshHelper->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
				SplineMeshHelper->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				SplineMeshHelper->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			}

			SplineMeshHelper->SetCullDistance(Builder->CullDistance);
		}break;

	case ESplineType::ST_MeshScaleCaps:
		if (Builder->StartCapMesh != nullptr)
		{
			StartCapExtent = FVector(Builder->StartCapMesh->GetBounds().GetBox().GetExtent()).X * 2;
		}
		
		if (Builder->EndCapMesh != nullptr)
		{
			EndCapExtent = FVector(Builder->EndCapMesh->GetBounds().GetBox().GetExtent()).X * 2;
		}

		lengthRemainder = FMath::Fmod(GetSplineLength() - StartCapExtent - EndCapExtent, Builder->SectionLength + Builder->Offset.Z);
		numPoints = FMath::TruncToInt((GetSplineLength() - StartCapExtent - EndCapExtent - Builder->Offset.Z)/(Builder->SectionLength));

		float distanceStart, distanceEnd;

		for (int32 Index = 0; Index < numPoints; Index++)
		{
			auto *SplineMeshHelper = NewObject <USplineMeshComponent>(Master);
			if (SplineMeshHelper == nullptr)
				return;

			SplineMeshHelper->CreationMethod = EComponentCreationMethod::Instance;
			SplineMeshHelper->SetMobility(EComponentMobility::Type::Stationary);//Type::Movable
			SplineMeshHelper->RegisterComponent();
			Builder->SplineMeshes.Add(SplineMeshHelper);

			//SplineMeshHelper->SetStaticMesh(Builder->DefaultMesh);

			/** TODO: Support Material Arrays */
			if (Builder->DefaultMaterialOverride != nullptr)
			{
				SplineMeshHelper->SetMaterial(0, Builder->DefaultMaterialOverride);

				UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(SplineMeshHelper->GetMaterial(0), SplineMeshHelper);
				if (MID != nullptr)
				{
					MID->SetScalarParameterValue(FName(TEXT("U Scale Multiplier")), Builder->TextureTilingX);
					MID->SetScalarParameterValue(FName(TEXT("V Scale Multiplier")), Builder->TextureTilingY);
					SplineMeshHelper->SetMaterial(0, MID);
				}
			}

			if (Builder->StartCapMesh != nullptr && Index == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("MultiSpline: StartCap %i"), Index);
				SplineMeshHelper->SetStaticMesh(Builder->StartCapMesh);

				distanceStart = Builder->Offset.Z;
				distanceEnd = Builder->Offset.Z + StartCapExtent;
			}
			else if (Builder->EndCapMesh != nullptr && Index == numPoints-1)
			{
				UE_LOG(LogTemp, Warning, TEXT("MultiSpline: EndCap %i"), Index);
				SplineMeshHelper->SetStaticMesh(Builder->EndCapMesh);

				distanceStart = Builder->Offset.Z + (Builder->SectionLength * Index) - EndCapExtent - lengthRemainder;
				distanceEnd = Builder->Offset.Z + (Builder->SectionLength * Index) - lengthRemainder;
				//distanceEnd = Builder->Offset.Z + Builder->SectionLength * (Index + 1);
				//distanceEnd = Builder->Offset.Z + Builder->SectionLength * (Index + 1) - FVector(Builder->EndCapMesh->GetBounds().GetBox().GetExtent()).X;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Stems %i"), Index);
				SplineMeshHelper->SetStaticMesh(Builder->DefaultMesh);

				distanceStart = StartCapExtent + Builder->Offset.Z + Builder->SectionLength * (Index-1);
				distanceEnd = StartCapExtent + Builder->Offset.Z + Builder->SectionLength * Index;
			}

			locStart = GetLocationAtDistanceAlongSpline(distanceStart, ESplineCoordinateSpace::Local);
			locStart.Z += Builder->Offset.X;
			locStart.Y += Builder->Offset.Y;

			tanStart = GetTangentAtDistanceAlongSpline(distanceStart, ESplineCoordinateSpace::Local);
			//tanStart = tanStart.GetClampedToSize(0, Builder->SectionLength);

			locEnd = GetLocationAtDistanceAlongSpline(distanceEnd, ESplineCoordinateSpace::Local);
			locEnd.Z += Builder->Offset.X;
			locEnd.Y += Builder->Offset.Y;

			tanEnd = GetTangentAtDistanceAlongSpline(distanceEnd, ESplineCoordinateSpace::Local);
			//tanEnd = tanEnd.GetClampedToSize(0, Builder->SectionLength);

			SplineMeshHelper->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, true);
			//Spline = FSplineConstruct(locStart, locEnd, tanStart, tanEnd, NewScale);

			/* Standard Collision if no Custom Mesh found */
			/*if (!BuildSplineMeshCollision(Master, BuilderScene, Spline, Builder->CollisionMesh, Index))
			{
				SplineMeshHelper->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
				SplineMeshHelper->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				SplineMeshHelper->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			}

			SplineMeshHelper->SetCullDistance(Builder->CullDistance);*/

			//SplineMeshHelper->SetStartScale(FVector2D(NewScale.X, NewScale.Y), true);
			//SplineMeshHelper->SetEndScale(FVector2D(NewScale.X, NewScale.Y), true);

			SplineMeshHelper->AttachToComponent(/*this*/ BuilderScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		}break;

	case ESplineType::ST_Segment:
		numPoints = GetNumberOfSplinePoints();

		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Segment - %d"), numPoints);

		for (int32 Index = 0; Index < numPoints - 1; Index++)
		{
			auto *SplineMeshHelper = NewObject <USplineMeshComponent>(Master);
			if (SplineMeshHelper == nullptr)
				return;

			SplineMeshHelper->CreationMethod = EComponentCreationMethod::Instance;
			SplineMeshHelper->SetMobility(EComponentMobility::Type::Stationary);
			SplineMeshHelper->RegisterComponent();
			Builder->SplineMeshes.Add(SplineMeshHelper);

			SplineMeshHelper->SetStaticMesh(Builder->DefaultMesh);

			SplineMeshHelper->SetStartScale(FVector2D(NewScale.X, NewScale.Y), true);
			SplineMeshHelper->SetEndScale(FVector2D(NewScale.X, NewScale.Y), true);

			/** TODO: Support Material Arrays */
			if (Builder->DefaultMaterialOverride != nullptr)
			{
				SplineMeshHelper->SetMaterial(0, Builder->DefaultMaterialOverride);

				UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(SplineMeshHelper->GetMaterial(0), SplineMeshHelper);
				if (MID != nullptr)
				{
					MID->SetScalarParameterValue(FName(TEXT("U Scale Multiplier")), Builder->TextureTilingX);
					MID->SetScalarParameterValue(FName(TEXT("V Scale Multiplier")), Builder->TextureTilingY);
					SplineMeshHelper->SetMaterial(0, MID);
				}
			}

			GetLocationAndTangentAtSplinePoint(Index, locStart, tanStart, ESplineCoordinateSpace::Local);
			locStart.X += Builder->Offset.X;
			locStart.Y += Builder->Offset.Y;

			//float rollStart = GetRollAtSplinePoint(Index, ESplineCoordinateSpace::Local);

			GetLocationAndTangentAtSplinePoint(Index + 1, locEnd, tanEnd, ESplineCoordinateSpace::Local);
			locEnd.X += Builder->Offset.X;
			locEnd.Y += Builder->Offset.Y;

			//float rollEnd = GetRollAtSplinePoint(Index + 1, ESplineCoordinateSpace::Local);

			SplineMeshHelper->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, true);
			Spline = FSplineConstruct(locStart, locEnd, tanStart, tanEnd, NewScale);

			/*if (Builder->bUseRoll)
			{
				SplineMeshHelper->SetStartRoll(rollStart);
				SplineMeshHelper->SetEndRoll(rollEnd);
			}*/

			SplineMeshHelper->AttachToComponent(/*this*/ BuilderScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			if (!BuildSplineMeshCollision(Master, BuilderScene, Spline, Builder->CollisionMesh, Index))
			{
				SplineMeshHelper->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
				SplineMeshHelper->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				SplineMeshHelper->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			}

			SplineMeshHelper->SetCullDistance(Builder->CullDistance);
		}break;

	case ESplineType::ST_SegmentTaper:
		numPoints = GetNumberOfSplinePoints();

		UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SegmentTaper - %d"), numPoints);

		for (int32 Index = 0; Index < numPoints - 1; Index++)
		{
			auto *SplineMeshHelper = NewObject <USplineMeshComponent>(Master);
			if (SplineMeshHelper == nullptr)
				return;

			SplineMeshHelper->CreationMethod = EComponentCreationMethod::Instance;
			SplineMeshHelper->SetMobility(EComponentMobility::Type::Stationary);
			SplineMeshHelper->RegisterComponent();
			Builder->SplineMeshes.Add(SplineMeshHelper);

			SplineMeshHelper->SetStaticMesh(Builder->DefaultMesh);

			float IndexPercent = float(Index + 1)*float(numPoints) / 100.0f;
			UE_LOG(LogTemp, Warning, TEXT("MultiSpline: %f"), IndexPercent);
			if (Index == 0)
			{
				SplineMeshHelper->SetStartScale(FVector2D(NewScale.X, NewScale.Y), true);
			}
			else
			{
				SplineMeshHelper->SetStartScale(PreviousScale, true);
			}
			PreviousScale.X = UKismetMathLibrary::MapRangeClamped(IndexPercent, 0.0f, 1.0f, NewScale.X, Builder->TaperScale.X);
			PreviousScale.Y = UKismetMathLibrary::MapRangeClamped(IndexPercent, 0.0f, 1.0f, NewScale.Y, Builder->TaperScale.Y);
			SplineMeshHelper->SetEndScale(PreviousScale, true);

			/** TODO: Support Material Arrays */
			if (Builder->DefaultMaterialOverride != nullptr)
			{
				SplineMeshHelper->SetMaterial(0, Builder->DefaultMaterialOverride);

				UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(SplineMeshHelper->GetMaterial(0), SplineMeshHelper);
				if (MID != nullptr)
				{
					MID->SetScalarParameterValue(FName(TEXT("U Scale Multiplier")), Builder->TextureTilingX);
					MID->SetScalarParameterValue(FName(TEXT("V Scale Multiplier")), Builder->TextureTilingY);
					SplineMeshHelper->SetMaterial(0, MID);
				}
			}

			GetLocationAndTangentAtSplinePoint(Index, locStart, tanStart, ESplineCoordinateSpace::Local);
			locStart.X += Builder->Offset.X;
			locStart.Y += Builder->Offset.Y;

			//FQuat GetQuaternionAtSplinePoint(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace) const;
			//float rollStart = GetRollAtSplinePoint(Index, ESplineCoordinateSpace::Local);

			GetLocationAndTangentAtSplinePoint(Index + 1, locEnd, tanEnd, ESplineCoordinateSpace::Local);
			locEnd.X += Builder->Offset.X;
			locEnd.Y += Builder->Offset.Y;

			//float rollEnd = GetRollAtSplinePoint(Index + 1, ESplineCoordinateSpace::Local);
			
			SplineMeshHelper->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, true);
			Spline = FSplineConstruct(locStart, locEnd, tanStart, tanEnd, NewScale, FVector(PreviousScale.X, PreviousScale.Y, 0.0f));

			/*if (Builder->bUseRoll)
			{
				SplineMeshHelper->SetStartRoll(rollStart);
				SplineMeshHelper->SetEndRoll(rollEnd);
			}*/

			SplineMeshHelper->AttachToComponent(/*this*/ BuilderScene, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			if (!BuildSplineMeshCollision(Master, BuilderScene, Spline, Builder->CollisionMesh, Index))
			{
				SplineMeshHelper->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
				SplineMeshHelper->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				SplineMeshHelper->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			}

			SplineMeshHelper->SetCullDistance(Builder->CullDistance);
		}break;

	case ESplineType::ST_Divide:
		numPoints = FMath::TruncToInt(GetSplineLength() / Builder->SectionLength + Builder->Offset.Z);
		auto remainder = FMath::TruncToInt(GetSplineLength()) % FMath::TruncToInt(Builder->SectionLength);
		float additionalSectionLength = remainder / numPoints;

		float scaleFactor = (Builder->SectionLength + additionalSectionLength) / Builder->SectionLength;

		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SectionLength - %f"), Builder.SectionLength);

		Builder->SectionLength *= scaleFactor;

		UE_LOG(LogTemp, Warning, TEXT("MultiSpline: SectionLength - %f"), Builder->SectionLength);
		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Divide - %d"), numPoints);

		for (int32 Index = 0; Index < numPoints - 1; Index++)
		{
			auto *SplineMeshHelper = NewObject <USplineMeshComponent>(Master);
			if (SplineMeshHelper == nullptr)
				return;

			SplineMeshHelper->CreationMethod = EComponentCreationMethod::Instance;
			SplineMeshHelper->SetMobility(EComponentMobility::Type::Movable);
			SplineMeshHelper->RegisterComponent();
			Builder->SplineMeshes.Add(SplineMeshHelper);

			SplineMeshHelper->SetStaticMesh(Builder->DefaultMesh);

			SplineMeshHelper->SetStartScale(FVector2D(NewScale.X, NewScale.Y), true);
			SplineMeshHelper->SetEndScale(FVector2D(NewScale.X, NewScale.Y), true);

			/** TODO: Support Material Arrays */
			if (Builder->DefaultMaterialOverride != nullptr)
			{
				SplineMeshHelper->SetMaterial(0, Builder->DefaultMaterialOverride);

				if (Builder->ExtrusionPreset != nullptr && Builder->ExtrusionPreset->TextureTilingX != 1.0f)
				{
					UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(SplineMeshHelper->GetMaterial(0), SplineMeshHelper);
					if (MID != nullptr)
					{
						MID->SetScalarParameterValue(FName(TEXT("U Scale Multiplier")), Builder->ExtrusionPreset->TextureTilingX);
						SplineMeshHelper->SetMaterial(0, MID);
					}
				}
			}

			distance = Builder->Offset.Z + Builder->SectionLength * Index;
			locStart = GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			locStart.Z += Builder->Offset.X;
			locStart.Y += Builder->Offset.Y;

			tanStart = GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			tanStart = tanStart.GetClampedToSize(0, Builder->SectionLength);

			//float rollStart = GetRollAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);

			distance = Builder->Offset.Z + Builder->SectionLength * (Index + 1);
			locEnd = GetLocationAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			locEnd.Z += Builder->Offset.X;
			locEnd.Y += Builder->Offset.Y;

			tanEnd = GetTangentAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);
			tanEnd = tanEnd.GetClampedToSize(0, Builder->SectionLength);

			//float rollEnd = GetRollAtDistanceAlongSpline(distance, ESplineCoordinateSpace::Local);

			SplineMeshHelper->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd, true);
			Spline = FSplineConstruct(locStart, locEnd, tanStart, tanEnd, NewScale);

			/*if (Builder->bUseRoll)
			{
				SplineMeshHelper->SetStartRoll(rollStart);
				SplineMeshHelper->SetEndRoll(rollEnd);
			}*/

			SplineMeshHelper->AttachToComponent(this, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

			/* Standard Collision if no Custom Mesh found */
			if (!BuildSplineMeshCollision(Master, BuilderScene, Spline, Builder->CollisionMesh, Index))
			{
				SplineMeshHelper->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
				SplineMeshHelper->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				SplineMeshHelper->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
			}

			SplineMeshHelper->SetCullDistance(Builder->CullDistance);
		}break;
	}
}

bool USplineBuilderComponent::BuildSplineMeshCollision(AActor* Master, USceneComponent* BuilderSceneComponent, FSplineConstruct Spline, UStaticMesh* CollisionMesh, uint32 index)
{
	if (Master == nullptr || CollisionMesh == nullptr)
		return false;

	FString MeshStringName = (TEXT("SplineMeshCollision_")) + FString::FromInt(index);
	FName MeshName = *MeshStringName;

	auto *SplineMeshCollision = NewObject <USplineMeshComponent>(Master, MeshName);
	if (SplineMeshCollision == nullptr)
		return false;

	SplineMeshCollision->CreationMethod = EComponentCreationMethod::Instance;

	SplineMeshCollision->SetStaticMesh(CollisionMesh);

	SplineMeshCollision->SetStartScale(FVector2D(Spline.ScaleStart.X, Spline.ScaleStart.Y), true);
	SplineMeshCollision->SetEndScale(FVector2D(Spline.ScaleEnd.X, Spline.ScaleEnd.Y), true);

	SplineMeshCollision->SetMobility(EComponentMobility::Type::Stationary);

	SplineMeshCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	SplineMeshCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineMeshCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	SplineMeshCollision->SetStartAndEnd(Spline.LocationStart, Spline.TangentStart, Spline.LocationEnd, Spline.TangentEnd, true);

	SplineMeshCollision->RegisterComponent();
	SplineMeshCollision->SetVisibility(false, false);
	//Builder.SplineMeshes.Add(SplineMeshCollision);
	SplineMeshCollision->AttachToComponent(/*this*/ BuilderSceneComponent, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

	return true;
}

void USplineBuilderComponent::ClearSplineMeshes()
{
	for (int32 SplineBuilderIndex = 0; SplineBuilderIndex < Builders.Num(); SplineBuilderIndex++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Remove Meshes %d"), Builders[SplineBuilderIndex].SplineMeshes.Num());
		for (int32 i = Builders[SplineBuilderIndex].SplineMeshes.Num() - 1; i > -1; i--)
		{
			if (Builders[SplineBuilderIndex].SplineMeshes[i] != nullptr)
			{
				Builders[SplineBuilderIndex].SplineMeshes[i]->DestroyComponent();
			}
			Builders[SplineBuilderIndex].SplineMeshes.RemoveAt(i);
			//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Builder - %d / Mesh - %d"), SplineBuilderIndex, i);
		}
		//Builders[SplineBuilderIndex].SplineMeshes.Empty();
	}
}

void USplineBuilderComponent::SnapToGround()
{
	auto* World = GetWorld();
	auto* Owner = GetOwner();
	if (World == nullptr || Owner == nullptr)
		return;

	for (int32 Index = 0; Index < GetNumberOfSplinePoints(); Index++)
	{
		FHitResult OutHit(ForceInit);
		FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("TraceWorld")), true, Owner);
		CollisionParams.bTraceComplex = true;
		//CollisionParams.bTraceAsyncScene = true;
		CollisionParams.bReturnPhysicalMaterial = false;

		FVector locStart = GetWorldLocationAtSplinePoint(Index);

		FVector traceEnd = FVector(locStart.X, locStart.Y, locStart.Z - FloorTraceDistance);

		auto *SplineTraceHelper = NewObject <USplineArrowComponent>(Owner);
		if (SplineTraceHelper == nullptr)
			return;

		//DrawDebugLine(World, locStart, traceEnd, FColor::Blue, false, -1.0f, 0, 1.0f);

		//UE_LOG(LogTemp, Warning, TEXT("Trace Start: %s, %s"), *locStart.ToCompactString(), *traceEnd.ToCompactString());

		/* Check Down */
		if (World->LineTraceSingleByChannel(OutHit, locStart, traceEnd, ECC_WorldStatic, CollisionParams))
		{
			if (OutHit.bBlockingHit)
			{
				DrawDebugLine(World, locStart, OutHit.ImpactPoint, FColor::Green, false, -1.0f, 0, 2.0f);
				DrawDebugLine(World, OutHit.ImpactPoint, traceEnd, FColor::Red, false, -1.0f, 0, 2.0f);

				SetWorldLocationAtSplinePoint(Index, FVector(locStart.X, locStart.Y, OutHit.ImpactPoint.Z + FloorOffset));

				ClearSplineMeshes();
				ExtrudeSplineMeshes();

				UE_LOG(LogTemp, Warning, TEXT("Trace: Ground Under Point!"));
			}
			else
			{
				DrawDebugLine(World, locStart, traceEnd, FColor::Red, false, -1.0f, 0, 2.0f);

				/* Check Up */
				FVector traceStart = FVector(locStart.X, locStart.Y, locStart.Z + FloorTraceDistance);
				//traceEnd = FVector(locStart.X, locStart.Y, locStart.Z);

				if (World->LineTraceSingleByChannel(OutHit, traceStart, locStart, ECC_WorldStatic, CollisionParams))
				{
					if (OutHit.bBlockingHit)
					{
						DrawDebugLine(World, locStart, OutHit.ImpactPoint, FColor::Green, false, -1.0f, 0, 2.0f);
						DrawDebugLine(World, OutHit.ImpactPoint, traceEnd, FColor::Red, false, -1.0f, 0, 2.0f);

						SetWorldLocationAtSplinePoint(Index, FVector(locStart.X, locStart.Y, OutHit.ImpactPoint.Z + FloorOffset));

						ClearSplineMeshes();
						ExtrudeSplineMeshes();

						UE_LOG(LogTemp, Warning, TEXT("Trace: Ground Above Point!"));
					}
					else
					{
						DrawDebugLine(World, locStart, traceEnd, FColor::Red, false, -1.0f, 0, 2.0f);
						UE_LOG(LogTemp, Warning, TEXT("Trace: No Ground Found!"));
					}
				}
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Initial Overlap: %s"), *GetWorld()->GetName());
		}
	}
}

void USplineBuilderComponent::ClearDebugLines()
{

}