/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "SplineGameInstance.generated.h"

//class USplineManager;

UCLASS()
class USplineGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	//FSplineExtrusionMap* SplineExtrusionMap;

	//USplineManager* SplineManager = nullptr;

	virtual void LoadComplete(const float LoadTime, const FString& MapName) override
	{
		Super::LoadComplete(LoadTime, MapName);
		UE_LOG(LogTemp, Warning, TEXT("GameInstance: LoadComplete"));
	}

	virtual void Init() override
	{
		Super::Init();
		UE_LOG(LogTemp, Warning, TEXT("GameInstance: Init"));
	}

	virtual ULocalPlayer*	CreateInitialPlayer(FString& OutError) override
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance: CreateInitalPlayer"));

		return Super::CreateInitialPlayer(OutError);
	}
};