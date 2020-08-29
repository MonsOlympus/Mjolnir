/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "SplineTool.h"
#include "SplineBuilderComponent.h"
#include "SplineExtrusionAsset.h"
#include "SplineManager.generated.h"

USTRUCT()
struct FSplineExtrusionList
{
	GENERATED_BODY()

public:
	TArray<ASplineTool*> Splines;
};

UCLASS()
class MJOLNIR_API USplineManager : public UObject
{
	GENERATED_BODY()

private:
	TArray <USplineExtrusionAsset*> Extrusions;

	TArray <FSplineExtrusionList> SplineMap;

public:

	//USplineManager();
	/*static TSharedPtr<FSplineExtrusionMap> GetSplineManager()
	{
		return MyPtr.MakeSharedRef();
		MyThing.SharedFromThis();
	}*/

	//TSharedPtr<something> myThing = MakeShared(someRawPtr);

	//void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	/* All created Splines are Registered with Manager */
	bool RegisterSplineTool(ASplineTool* Spline) 
	{ 
		//UE_LOG(LogTemp, Warning, TEXT("SplineManager: Register"));

		int32 Index = -1;

		if (Spline != nullptr && Spline->FirstBuilder != nullptr && Spline->FirstBuilder->Builders.Num() > 0 &&
			Spline->FirstBuilder->Builders[0].ExtrusionPreset != nullptr)
		{
			USplineExtrusionAsset* TempExtrusion = Spline->FirstBuilder->Builders[0].ExtrusionPreset;
			//UE_LOG(LogTemp, Warning, TEXT("SplineManager: Temp Extrusion %s"), *GetNameSafe(TempExtrusion));
			if (TempExtrusion != nullptr)
			{
				Index = Extrusions.Find(TempExtrusion);
				if (Index > -1)
				{
					//UE_LOG(LogTemp, Warning, TEXT("SplineManager: Temp Extrusion Contains"));
					SplineMap[Index].Splines.Add(Spline);
					return true;
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("SplineManager: Temp Extrusion New Add"));
					Index = Extrusions.Add(TempExtrusion);
					SplineMap.EmplaceAt(Index);
					SplineMap[Index].Splines.Add(Spline);
					return true;
				}
			}
			UE_LOG(LogTemp, Error, TEXT("SplineManager: Failed to Register: %s"), *GetNameSafe(Spline));
		}
		return false;
	}

	bool UnregisterSplineTool(ASplineTool* Spline) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("SplineManager: Unregister"));

		if (Spline != nullptr && Spline->FirstBuilder != nullptr && Spline->FirstBuilder->Builders.Num() > 0 &&
			Spline->FirstBuilder->Builders[0].ExtrusionPreset != nullptr)
		{
			USplineExtrusionAsset* TempExtrusion = Spline->FirstBuilder->Builders[0].ExtrusionPreset;
			if (TempExtrusion != nullptr && Extrusions.Contains(TempExtrusion))
			{
				int32 IndexA = Extrusions.Find(TempExtrusion);
				int32 IndexB = SplineMap[IndexA].Splines.Find(Spline);
				SplineMap[IndexA].Splines.RemoveAt(IndexB);
				if (SplineMap[IndexA].Splines.Num() == 0)
				{
					SplineMap.RemoveAt(IndexA);
					Extrusions.RemoveAt(IndexB);
				}
				return true;
			}
			UE_LOG(LogTemp, Error, TEXT("SplineManager: Failed to Unregister: %s"), *GetNameSafe(Spline));
		}
		return false;
	}

	/* Any changes to Spline Extrusions will call this function and subsiquently Regenerate Splines*/
	void UpdateExtrusions(USplineExtrusionAsset* Extrusion)
	{
		if (Extrusion != nullptr && Extrusions.Contains(Extrusion))
		{
			int32 Index = Extrusions.Find(Extrusion);
			for (auto Spline : SplineMap[Index].Splines)
			{
				Spline->RegenerateSpline();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SplineManager: Problem Updating Extrusions"));
		}
	}

//protected:
	//virtual void BeginDestroy() override;
};