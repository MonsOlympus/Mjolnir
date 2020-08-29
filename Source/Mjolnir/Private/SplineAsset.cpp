/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#include "SplineAsset.h"
#include "Math/InterpCurve.h"
#include "JsonUtilities.h"

void USplineAsset::LoadSpline(FSplineCurves& SplineCurves)
{
	if (FJsonObjectConverter::JsonObjectStringToUStruct(SplineJSON.ToString(), &SplineCurves, 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("MultiSpline: Loaded Spline!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MultiSpline: Cannot Load Spline!"));
	}
}

void USplineAsset::SaveSpline(FSplineCurves SplineCurves)
{
	FString OutputString = "";
	if (FJsonObjectConverter::UStructToJsonObjectString(SplineCurves, OutputString))
	{
		SplineJSON = FText::FromString(OutputString);
		MarkPackageDirty();
	}
}

//TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

//SplineObject->SetStringField("Name", "SplineTest");
//SplineObject->SetNumberField("Version", 0.15);

/*FInterpCurveVector Position;
FInterpCurveQuat Rotation;
FInterpCurveVector Scale;*/

//SplineCurves.Position.Points;

//FJsonObjectConverter::JsonObjectStringToUStruct<FMain>(JsonString, &Main, 0, 0);

	//UE_LOG(LogTemp, Error, TEXT("MultiSpline: %s"), *OutString);

//	FJsonValue;

	//JsonObject->SetArrayField("", );

	//JsonObject->SetObjectField("Spline", SplineObject);

	//FString OutputString;
	//TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);