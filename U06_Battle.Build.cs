// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class U06_Battle : ModuleRules
{
	public U06_Battle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("InputCore");

        // Add
        PublicDependencyModuleNames.Add("Niagara");
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("GameplayTasks");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("ProceduralMeshComponent");  // PlugIN
        PublicDependencyModuleNames.Add("LevelSequence");
        PublicDependencyModuleNames.Add("HeadMountedDisplay");
        PublicDependencyModuleNames.Add("MovieScene");
        PublicDependencyModuleNames.Add("SlateCore");
        PublicDependencyModuleNames.Add("Slate");
        PublicDependencyModuleNames.Add("UMG");
    }
}
