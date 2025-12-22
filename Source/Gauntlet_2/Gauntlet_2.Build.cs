// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Gauntlet_2 : ModuleRules
{
	public Gauntlet_2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Gauntlet_2",
			"Gauntlet_2/Variant_Platforming",
			"Gauntlet_2/Variant_Platforming/Animation",
			"Gauntlet_2/Variant_Combat",
			"Gauntlet_2/Variant_Combat/AI",
			"Gauntlet_2/Variant_Combat/Animation",
			"Gauntlet_2/Variant_Combat/Gameplay",
			"Gauntlet_2/Variant_Combat/Interfaces",
			"Gauntlet_2/Variant_Combat/UI",
			"Gauntlet_2/Variant_SideScrolling",
			"Gauntlet_2/Variant_SideScrolling/AI",
			"Gauntlet_2/Variant_SideScrolling/Gameplay",
			"Gauntlet_2/Variant_SideScrolling/Interfaces",
			"Gauntlet_2/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
