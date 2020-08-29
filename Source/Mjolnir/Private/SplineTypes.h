/*
	2016-2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "Internationalization/Text.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"

#include "SplineTypes.generated.h"

UENUM(BlueprintType)
enum class ESplineType : uint8
{
	ST_Segment,
	ST_SegmentTaper,
	ST_MeshScale,
	ST_MeshScaleSlice,
	ST_MeshScaleCaps,
	ST_MeshScaleTaper,
	ST_Divide,
	ST_DivideCaps
};

UENUM(BlueprintType)
enum class EPlaceType : uint8
{
	PT_Spline,
	PT_Segment,
	PT_Gap,
	PT_Scatter,
	PT_Socket,
	PT_SmartScatter
};

/** Replacement on a Per Map basis - Get from global when not found */
USTRUCT(BlueprintType)
struct FMapReplacer
{
	GENERATED_BODY()

	FString Level;					/// Map name, this is not a unique id!
	uint8 LevelID = 0;				/// This is so multiple entries can be chosen per map.
};

USTRUCT(BlueprintType)
struct FSplinePointBuilder
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	UStaticMesh* DefaultSplinePointMesh;

	UPROPERTY(EditAnywhere)
	FTransform PreTransform;
};

USTRUCT(BlueprintType)
struct FSplineNode
{
	GENERATED_BODY()

	/** Handy for things like posts or joins */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	UBlueprint* NodeClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	FVector LocationFromSpline;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	float DistanceAlongSpline;
};

USTRUCT(BlueprintType)
struct FSplinePointCap
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Caps")
	UStaticMesh* StartCapMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Caps")
	UStaticMesh* EndCapMesh;
};

USTRUCT(BlueprintType)
struct FSplineSpacer
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	FVector SpacingMinimum;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Defaults")
	FVector SpacingMaximum;
};

USTRUCT()
struct FSplineConstruct
{
	GENERATED_BODY()

	FVector LocationStart;
	FVector LocationEnd;
	FVector TangentStart;
	FVector TangentEnd;
	FVector ScaleStart;
	FVector ScaleEnd;

	FSplineConstruct()
	{
	}

	FSplineConstruct(FVector locStart, FVector locEnd, FVector tanStart, FVector tanEnd, FVector Scale)
	{
		LocationStart = locStart;
		LocationEnd = locEnd;
		TangentStart = tanStart;
		TangentEnd = tanEnd;
		ScaleStart = Scale;
		ScaleEnd = Scale;
	}

	FSplineConstruct(FVector locStart, FVector locEnd, FVector tanStart, FVector tanEnd, FVector scaStart, FVector scaEnd)
	{
		LocationStart = locStart;
		LocationEnd = locEnd;
		TangentStart = tanStart;
		TangentEnd = tanEnd;
		ScaleStart = scaStart;
		ScaleEnd = scaEnd;
	}
};

USTRUCT(BlueprintType)
struct FRandomTransform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FRotator RotationMin;

	UPROPERTY(EditAnywhere)
	FRotator RotationMax;

	UPROPERTY(EditAnywhere)
	FVector LocationMin;

	UPROPERTY(EditAnywhere)
	FVector LocationMax;

	UPROPERTY(EditAnywhere)
	FVector ScaleMin;

	UPROPERTY(EditAnywhere)
	FVector ScaleMax;
};