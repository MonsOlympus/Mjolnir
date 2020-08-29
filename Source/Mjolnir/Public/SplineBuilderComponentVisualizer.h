/*
	2016-2018 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "SplineInterface.h"
#include "SplineBuilderComponentVisualizer.generated.h"

class USceneComponent;

class MJOLNIR_API FSplineBuilderComponentVisualizer : public FSplineComponentVisualizer
{
public:
	//FSplineBuilderComponentVisualizer();

protected:
	void ChangeSelectionState(int32 Index, bool bIsCtrlHeld) override;
	//void OnAddKey() override;
}