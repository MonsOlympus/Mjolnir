/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#include "SplineTool.h"
#include "SplineManager.h"
#include "Components/SplineMeshComponent.h"

class FMjolnir;
class UStaticMesh;
class USplineBuilderComponent;

ASplineTool::ASplineTool()
	: Super()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = Scene;
	RootComponent->SetMobility(EComponentMobility::Type::Stationary);
	
	this->SetActorEnableCollision(true);

	FirstBuilder = CreateDefaultSubobject<USplineBuilderComponent>(TEXT("SplineBuilder"));
	if (FirstBuilder != nullptr && RootComponent != nullptr)
	{
		FirstBuilder->SetupAttachment(RootComponent);

		if (GetWorld() != nullptr)
		{
			FirstBuilder->RegisterComponent();
		}

		//FirstBuilder->AttachToComponent(RootComponent, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		//FirstBuilder->SetWorldLocation(RootComponent->GetComponentToWorld().GetLocation(), false, NULL, ETeleportType::None);
	}

	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: C++ Constructor"));
}

void ASplineTool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FMjolnir& MjolnirModule = FModuleManager::GetModuleChecked<FMjolnir>("Mjolnir");
	if (MjolnirModule.SplineManager != nullptr)
	{
		//UE_LOG(LogClass, Error, TEXT("SplineTool: SplineManager Found!"));
		MjolnirModule.SplineManager->RegisterSplineTool(this);
	}

	UBlueprint* Blueprint = Cast<UBlueprint>(GetClass()->ClassGeneratedBy);
	if (Blueprint)
		Blueprint->bRunConstructionScriptOnDrag = true;

	RegenerateSpline();

	/*TArray<USplineBuilderComponent*> SplineBuilderComps;
	GetComponents(SplineBuilderComps, false);

	for (USplineBuilderComponent* BuilderComp : SplineBuilderComps)
	{
		//BuilderComp->ClearSplineMeshes();
		//BuilderComp->ExtrudeSplineMeshes();

		//BuilderComp->ClearSplinePlacers();
		//BuilderComp->PlaceMeshesAlongSpline();

		UE_LOG(LogTemp, Warning, TEXT("MultiSpline: OnConstruction RegenerateSplines"));
	}*/
	   
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: BP OnConstruction after ExtrudeSplineMeshes"));

	if (GetWorld() != nullptr)
	{
		this->RegisterAllComponents();
	}
}

void ASplineTool::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		RegenerateSpline();
		UE_LOG(LogTemp, Error, TEXT("MultiSpline: %s: Property Edited !"), *GetNameSafe(this));
	}
}

/*void ASplineTool::PostInitProperties()
{
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: PostInit"));
	Super::PostInitProperties();

	FMjolnir& MjolnirModule = FModuleManager::LoadModuleChecked<FMjolnir>("Mjolnir");
	if (MjolnirModule.SplineManager != nullptr)
	{
		MjolnirModule.SplineManager->RegisterSplineTool(this);
	}
}*/

void ASplineTool::RegenerateSpline()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Regenerate"));
	
	//TInlineComponentArray<USceneComponent*> Components;
	TArray<USplineMeshComponent*> SplineMeshes;
	GetComponents(SplineMeshes, false);

	for (USplineMeshComponent* SplineMesh : SplineMeshes)
	{
		SplineMesh->DestroyComponent();
		//BuilderComp->ClearSplineMeshes();
	}

	TArray<UInstancedStaticMeshComponent*> PlacedMeshes;
	GetComponents(PlacedMeshes, false);
	
	for (UInstancedStaticMeshComponent* PlacedMesh : PlacedMeshes)
	{
		PlacedMesh->DestroyComponent();
	}

	TArray<USplineBuilderComponent*> SplineBuilderComps;
	GetComponents(SplineBuilderComps, false);

	for (USplineBuilderComponent* BuilderComp : SplineBuilderComps)
	{
		//BuilderComp->ClearSplineMeshes();
		//BuilderComp->ClearSplinePlacers();

		BuilderComp->ExtrudeSplineMeshes();
		BuilderComp->PlaceMeshesAlongSpline();
	}

	if (GetWorld() != nullptr)
	{
		this->RegisterAllComponents();
	}
}

void ASplineTool::BeginDestroy()
{
	FMjolnir& MjolnirModule = FModuleManager::GetModuleChecked<FMjolnir>("Mjolnir");
	if (MjolnirModule.SplineManager != nullptr)
	{
		//UE_LOG(LogClass, Error, TEXT("SplineTool: SplineManager Found!"));
		MjolnirModule.SplineManager->UnregisterSplineTool(this);
	}
	Super::BeginDestroy();
}

void ASplineTool::SnapAllToGround()
{
	TArray<USplineBuilderComponent*> SplineBuilderComps;
	GetComponents(SplineBuilderComps, false);

	for (USplineBuilderComponent* BuilderComp : SplineBuilderComps)
	{
		BuilderComp->SnapToGround();
	}
}

/*void ASplineTool::SaveSplineAsset()
{
	UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Save Asset"));

	if (SplinePreset != nullptr)
	{
		//SplinePreset->SaveSpline();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MultiSpline: Cannot save, no Spline Preset selected!"));
	}
}*/

/*void ASplineTool::AddNewBuilder()
{
	if (GetWorld() == nullptr)
		return;

	USplineBuilderComponent* newBuilder = NewObject<USplineBuilderComponent>();
	if (newBuilder != nullptr)
	{
		//newBuilder->SetMobility(EComponentMobility::Type::Stationary);
		newBuilder->SetupAttachment(RootComponent);
		newBuilder->CreationMethod = EComponentCreationMethod::Instance;

		//newBuilder->RegisterComponent();
	}
}*/

/*void ASplineTool::AddNewEntryToBuilder()
{
	//FSplineBuilder tempBuilder = FSplineBuilder(DefaultMesh, MaterialOverride);
//if (tempBuilder != NULL)
//{
	//FirstBuilder->Builders.Add(tempBuilder);
//}

	if (SplinePreset != nullptr)
	{
		new(FirstBuilder->Builders) FSplineBuilder();
	}
}*/

/*bool USplineHelperComponent::SerializeData(FString NewFileName, FString SaveText = FString(TEXT("")))
{
	NewFileName += TEXT("_splines.json");

	//IFileHandle * pFile = FPlatformFileManager::Get().GetPlatformFile().OpenWrite(*filename);
	if (UFileToolsBlueprintLibrary::FileExists(NewFileName))
	{
		UFileToolsBlueprintLibrary::FileSaveString(SaveText, NewFileName);
		return true;
	}
	else
	{
		//Create File

		return true;
	}

	return false;
}

FString USplineHelperComponent::DeSerializeData(FString ExistingFileName)
{
	FString& SaveText;

	if (UFileToolsBlueprintLibrary::FileExists(NewFileName))
	{
		UFileToolsBlueprintLibrary::FileLoadString(ExistingFileName, SaveText);
	}

	return SaveText;
}*/

/*bool ASplineTool::CheckSplineForJunctions()
{
	//TODO: Crosscheck spline points to make sure non-overlap
	return false;
}*/