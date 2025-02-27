// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

using UnrealBuildTool;

public class ElizaEditor : ModuleRules
{
	public ElizaEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		
        PrivateDependencyModuleNames.AddRange(
			new string[] {
                "Core",
				"CoreUObject",
				"Engine", 
				"RHI",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"UnrealEd",
				"Projects"
			}
		);

		PublicDependencyModuleNames.AddRange(
		new string[] { 
			"Eliza" 
			}
		);
	}
	
}
