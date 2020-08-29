namespace UnrealBuildTool.Rules
{
    public class Mjolnir : ModuleRules
    {
        public Mjolnir(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

            //PublicIncludePaths.AddRange(new string[] {"Mjolnir/Public/"} );
            //PrivateIncludePaths.AddRange(new string[] {"Mjolnir/Private/"} );

            DynamicallyLoadedModuleNames.AddRange(
            new string[] 
            {
                "AssetTools",
                "MainFrame",
       			//"BlueTux",
            });

            PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
                "Core",
                "CoreUObject",
                "InputCore",
                "Slate",
                "EditorStyle",
                //"AIModule",
                "BlueprintGraph",
                "MessageLog",
                "Json",
                "JsonUtilities",
                "Mjolnir"
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
                    "PropertyEditor",
                    "Mjolnir"
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
}
