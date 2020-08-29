/*
	2005-2018 Daniel Batten (MonsOlympus)
	Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
	https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/
#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
//#include "Curves/CurveOwnerInterface.h"
//#include "SCurveEditor.h"

class IDetailLayoutBuilder;
class USplineBuilderComponent;

class FSplineBuilderComponentDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	~FSplineBuilderComponentDetails();


private:

	/*struct FSteeringCurveEditor : public FCurveOwnerInterface
	{
		FSteeringCurveEditor(UWheeledVehicleMovementComponent4W * InVehicle = NULL);

		virtual TArray<FRichCurveEditInfoConst> GetCurves() const override;
		virtual TArray<FRichCurveEditInfo> GetCurves() override;
		virtual void ModifyOwner() override;
		virtual TArray<const UObject*> GetOwners() const override;
		virtual void MakeTransactional() override;
		virtual void OnCurveChanged(const TArray<FRichCurveEditInfo>& ChangedCurveEditInfos) override { }
		virtual bool IsValidCurve(FRichCurveEditInfo CurveInfo) override;

	private:
		UWheeledVehicleMovementComponent4W * VehicleComponent;
		UObject* Owner;

	} SteeringCurveEditor;*/

	/*struct FTorqueCurveEditor : public FCurveOwnerInterface
	{
		FTorqueCurveEditor(UWheeledVehicleMovementComponent4W * InVehicle = NULL);
		
		virtual TArray<FRichCurveEditInfoConst> GetCurves() const override;
		virtual TArray<FRichCurveEditInfo> GetCurves() override;
		virtual void ModifyOwner() override;
		virtual TArray<const UObject*> GetOwners() const override;
		virtual void MakeTransactional() override;
		virtual void OnCurveChanged(const TArray<FRichCurveEditInfo>& ChangedCurveEditInfos) override { }
		virtual bool IsValidCurve(FRichCurveEditInfo CurveInfo) override;

	private:
		UWheeledVehicleMovementComponent4W * VehicleComponent;
		UObject* Owner;

	} TorqueCurveEditor;*/

	TArray<TWeakObjectPtr<UObject> > SelectedObjects;	//the objects we're showing details for

	/** Steering curve widget */
	//TSharedPtr<class SCurveEditor> SteeringCurveWidget;

	/** Torque curve widget */
	//TSharedPtr<class SCurveEditor> TorqueCurveWidget;
}