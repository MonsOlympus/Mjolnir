#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "SplineAssetFactory.generated.h"


/**
 * Implements a factory for UTextAsset objects.
 */
UCLASS(hidecategories = Object)
class USplineAssetFactory : public UFactory
{
	GENERATED_BODY()

public:

	//~ UFactory Interface
	USplineAssetFactory(const FObjectInitializer& ObjectInitializer);

//	virtual UObject* FactoryCreateBinary(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, const TCHAR* Type, const uint8*& Buffer, const uint8* BufferEnd, FFeedbackContext* Warn) override;
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface

	virtual uint32 GetMenuCategories() const override;
	virtual FText GetDisplayName() const override;
	virtual FString GetDefaultNewAssetName() const override;

	virtual bool ShouldShowInNewMenu() const override;
};