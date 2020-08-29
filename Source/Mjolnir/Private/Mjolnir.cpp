#include "Containers/Array.h"
#include "Templates/SharedPointer.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
//#include "IPlacementModeModule.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "SplineAssetActions.h"
#include "SplineAssetEditorStyle.h"
#include "SplineAssetEditorSettings.h"

#include "SplineManager.h"

#define LOCTEXT_NAMESPACE "Mjolnir"

class USplineManager;
class ASplineTool;

class FMjolnir : public IHasMenuExtensibility
	, public IHasToolBarExtensibility
	, public IModuleInterface
{
public:
	static inline FMjolnir& Get()
	{
		return FModuleManager::LoadModuleChecked<FMjolnir>("Mjolnir");
	}

	//~ IHasMenuExtensibility interface
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override
	{
		return MenuExtensibilityManager;
	}

	//~ IHasToolBarExtensibility interface
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override
	{
		return ToolBarExtensibilityManager;
	}

	/*static inline TSharedPtr<USplineManager> GetSplineManager()
	{
		return SplineManager;
	}*/

	EAssetTypeCategories::Type SplinesAssetCategory;

	//~ IModuleInterface interface
	virtual void StartupModule() override
	{
		Style = MakeShareable(new FSplineAssetEditorStyle());

		//		FTextAssetEditorCommands::Register();

		//Register Asset Types
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		SplinesAssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Mjolnir")), LOCTEXT("MjolnirCategory", "Splines"));

		AssetTools.RegisterAssetTypeActions(MakeShareable(new FSplineAssetActions(Style.ToSharedRef())));

			// register AI category so that AI assets can register to it
			//AIAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("AI")), LOCTEXT("AIAssetCategory", "Artificial Intelligence"));
		
			//FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			//PropertyModule.RegisterCustomClassLayout("Dialogue", FOnGetDetailCustomizationInstance::CreateStatic(&FDialoguePluginEditorSettingsDetails::MakeInstance));
			//PropertyModule.NotifyCustomizationModuleChanged();

		RegisterMenuExtensions();
		RegisterSettings();

		UE_LOG(LogTemp, Warning, TEXT("Mjolnir: StartupModule"));

		//NewSplineManager();
		//SplineManager = MakeShared<USplineManager>();
		SplineManager = NewObject<USplineManager>();
	}

	virtual void ShutdownModule() override
	{
		UnregisterAssetTools();
		UnregisterMenuExtensions();
		UnregisterSettings();
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

private:
/*	template<typename T>
	void RegisterPlacementModeExtension(IPlacementModeModule& PlacementModeModule, UActorFactory* Factory = nullptr)
	{
		PlacementModeModule.RegisterPlaceableItem(PlacementCategoryInfo.UniqueHandle, MakeShared<FPlaceableItem>(Factory, FAssetData(T::StaticClass())));
	}
	void RegisterPlacementModeExtensions()
	{
		IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
		PlacementModeModule.RegisterPlacementCategory(PlacementCategoryInfo);

		//RegisterPlacementModeExtension<AVoxelWorld       >(PlacementModeModule, GetMutableDefault<UActorFactoryVoxelWorld       >());

		PlacementModeModule.RegenerateItemsForCategory(FBuiltInPlacementCategories::AllClasses());
	}
	void UnregisterPlacementModeExtensions()
	{
		if (IPlacementModeModule::IsAvailable())
		{
			IPlacementModeModule::Get().UnregisterPlacementCategory(PlacementCategoryInfo.UniqueHandle);
		}
	}*/

protected:

	/** Register the text asset editor settings. */
	void RegisterSettings()
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Editor", "Plugins", "SplineAsset",
				LOCTEXT("SplineAssetSettingsName", "Spline Asset"),
				LOCTEXT("SplineAssetSettingsDescription", "Configure the Spline Asset plug-in."),
				GetMutableDefault<USplineAssetEditorSettings>()
			);
		}
	}

	/** Unregisters asset tool actions. */
	void UnregisterAssetTools()
	{
		FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

		if (AssetToolsModule != nullptr)
		{
			IAssetTools& AssetTools = AssetToolsModule->Get();

			for (auto Action : RegisteredAssetTypeActions)
			{
				AssetTools.UnregisterAssetTypeActions(Action);
			}
		}
	}

	/** Unregister the text asset editor settings. */
	void UnregisterSettings()
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->UnregisterSettings("Editor", "Plugins", "SplineAsset");
		}
	}

protected:

	/** Registers main menu and tool bar menu extensions. */
	void RegisterMenuExtensions()
	{
		MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
	}

	/** Unregisters main menu and tool bar menu extensions. */
	void UnregisterMenuExtensions()
	{
		MenuExtensibilityManager.Reset();
		ToolBarExtensibilityManager.Reset();
	}

private:

	/** Holds the menu extensibility manager. */
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;

	/** The collection of registered asset type actions. */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

	/** Holds the plug-ins style set. */
	TSharedPtr<ISlateStyle> Style;

	/** Holds the tool bar extensibility manager. */
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;

//private:
	//static TWeakObjectPtr<class USplineManager> SplineManager;

	//static USplineManager* SplineManager;
	//static TSharedPtr<USplineManager> SplineManager;
public:
	USplineManager* SplineManager;
	//TSharedRef<USplineManager> SplineManager = MakeShared<USplineManager>();
	/*static USplineManager* GetSplineManager(){ 
		return FMjolnir::SplineManager.Get();
	}*/
	/*static USplineManager* GetSplineManager() {
		return &FMjolnir::SplineManager;
	}*/

	/*void NewSplineManager(){
		USplineManager* TempSplineManager = GetSplineManager();
		TempSplineManager = NewObject <USplineManager>();
		TempSplineManager->AddToRoot();
		//FMjolnir::SplineManager = MakeShared <USplineManager>();
		//FMjolnir::SplineManager = MakeShareable(new USplineManager);
	}*/
};

IMPLEMENT_MODULE(FMjolnir, Mjolnir)

#undef LOCTEXT_NAMESPACE