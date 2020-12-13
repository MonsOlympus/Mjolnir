using UnrealBuildTool;

public class Mjolnir : ModuleRules
{
    public Mjolnir(ReadOnlyTargetRules Target) : base(Target)
    {
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        DynamicallyLoadedModuleNames.AddRange(
            new string[] 
            {
                "AssetTools",
                "MainFrame",
                "PropertyEditor"
            });

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "AppFramework",
                "Engine",
                "Core",
                "CoreUObject",
                "InputCore",
                "Slate",
                "EditorStyle",
                //"PropertyEditor",
                //"AIModule",
                "BlueprintGraph",
                "MessageLog",
                "Json",
                "JsonUtilities",
                "ToolMenus",
                "UnrealEd",
                //"Mjolnir"
               });

            PrivateDependencyModuleNames.AddRange(
            new string[]
                {
                    "ContentBrowser",
                    "Core",
                    "CoreUObject",
                    "DesktopWidgets",
                    "EditorStyle",
                    "Engine",
                    "InputCore",
                    "Projects",
                    "Slate",
                    "SlateCore",
                     "UnrealEd",
                    "KismetCompiler",
                    "GraphEditor",
                    "Kismet",
                    "KismetWidgets",
                    //"PropertyEditor",
                    //"Mjolnir"
                }
            );
            PrivateIncludePathModuleNames.AddRange(
                new string[] 
                {
                    "AssetTools",
                    "UnrealEd",
                    //"Mjolnir/Private"
        //				"WorkspaceMenuStructure",
             }
        );
    }
}
