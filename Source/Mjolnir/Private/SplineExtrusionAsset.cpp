/*
	2019 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#include "SplineExtrusionAsset.h"

#if WITH_EDITOR
void USplineExtrusionAsset::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Changed Property"));

	if (PropertyChangedEvent.Property != nullptr)
	{
		//static const FName BuildersName = GET_MEMBER_NAME_CHECKED(USplineBuilderComponent, Builders);
		
		//const FName PropertyName(PropertyChangedEvent.Property->GetFName());

		//UE_LOG(LogTemp, Warning, TEXT("MultiSpline: Edited Property %s"), *PropertyName.ToString());
		/*if (PropertyName == BuildersName ||	 PropertyName == ExtrusionPresetName)
		{
			ClearSplineMeshes();
		}*/

		UE_LOG(LogTemp, Error, TEXT("SplineExtrusionAsset: Extrusion Edited"));

		FMjolnir& MjolnirModule = FModuleManager::GetModuleChecked<FMjolnir>("Mjolnir");
		if (MjolnirModule.SplineManager != nullptr)
		{
			UE_LOG(LogClass, Error, TEXT("SplineExtrusionAsset: SplineManager Found!"));
			MjolnirModule.SplineManager->UpdateExtrusions(this);
		}
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif //WITH_EDITOR