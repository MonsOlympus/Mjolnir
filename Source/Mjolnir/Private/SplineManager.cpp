/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
//#include "SplineManager.h"
//#include "Kismet/GameplayStatics.h"

//class USplineBuilderComponent;

/*USplineManager::USplineManager()
	: Super()
{
	//FWorldDelegates::FWorldInitializationEvent
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &USplineManager::OnWorldInitialized);
}*/

/*void USplineManager::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
	UE_LOG(LogTemp, Warning, TEXT("SplineTool: Startup"));

	if (World != nullptr)
	{
		TArray<AActor*> Splines;
		UGameplayStatics::GetAllActorsOfClass(World, ASplineTool::StaticClass(), Splines);
		UE_LOG(LogTemp, Warning, TEXT("SplineTool: %d"), Splines.Num());

		// Build Spline Extrusion Map
		for (int32 Index = 0; Index >= Splines.Num(); Index++)
		{
			const auto TempSpline = Cast<ASplineTool>(Splines[Index]);
			if (TempSpline != nullptr)
			{
				SplineExtrusionMap.Add(TempSpline);
			}
		}

		*//* Build TMap */
/*		SplineExtrusions.Empty();
		for (int32 Index = 0; Index >= Splines.Num(); Index++)
		{
			const auto TempSpline = Cast<ASplineTool>(Splines[Index]);
			if (TempSpline != nullptr)
			{
				if (TempSpline->FirstBuilder->Builders[0].ExtrusionPreset != nullptr)
				{
					TPair<TSubclassOf<USplineExtrusionAsset>, ASplineTool* > NewSpline;
					NewSpline.Key = TempSpline->FirstBuilder->Builders[0].ExtrusionPreset->GetClass();
					NewSpline.Value = TempSpline;
					SplineExtrusions.Add(NewSpline);
				}
			}
		}*//*
	}
}*/

//bool USplineManager::RegisterSplineTool(ASplineTool* Spline)
//{
//	return SplineExtrusionMap.Add(Spline);

	/*const auto TempSpline = Cast<ASplineTool>(Spline);
	if (TempSpline != nullptr)
	{
		if (TempSpline->FirstBuilder->Builders[0].ExtrusionPreset != nullptr)
		{
			TPair<TSubclassOf<USplineExtrusionAsset>, ASplineTool* > NewSpline;
			NewSpline.Key = TempSpline->FirstBuilder->Builders[0].ExtrusionPreset->GetClass();
			NewSpline.Value = TempSpline;
			SplineExtrusions.Add(NewSpline);
			return true;
		}
	}
	return false;*/
//}

//bool USplineManager::UnregisterSplineTool(ASplineTool* Spline)
//{
//	return SplineExtrusionMap.Remove(Spline);
	/*const auto TempSpline = Cast<ASplineTool>(Spline);
	if (TempSpline != nullptr)
	{
		bool bFound;
		int32 FoundIndex = 0;

		for (int32 Index = 0; Index < SplineExtrusions.Num(); Index++)
		{
			if (Spline->FirstBuilder->Builders[0].ExtrusionPreset == SplineExtrusions[Index].Key)
			{
				if (Spline == SplineExtrusions[Index].Value)
				{
					bFound = true;
					FoundIndex = Index;
				}
			}
		}
		if (bFound)
		{
			SplineExtrusions.Remove(FoundIndex);
			return true;
		}
	}
	return false;*/
//}

//void USplineManager::UpdateExtrusions(TSubclassOf <USplineExtrusionAsset> Extrusion)
//{
//	SplineExtrusionMap.RegenerateSplines(Extrusion);
	/*for (const TPair<TSubclassOf<USplineExtrusionAsset>, ASplineTool* >& Spline : SplineExtrusions)
	{
		if (Spline.Key == Extrusion.GetClass())
		{
			Spline.Value->RegenerateSpline();
		}
	}*/
//}

/*void USplineManager::BeginDestroy()
{
	FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);

	Super::BeginDestroy();
}*/