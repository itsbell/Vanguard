// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Vanguard : ModuleRules
{
	public Vanguard(ReadOnlyTargetRules Target) : base(Target)
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
			"Vanguard",
			"Vanguard/Variant_Platforming",
			"Vanguard/Variant_Platforming/Animation",
			"Vanguard/Variant_Combat",
			"Vanguard/Variant_Combat/AI",
			"Vanguard/Variant_Combat/Animation",
			"Vanguard/Variant_Combat/Gameplay",
			"Vanguard/Variant_Combat/Interfaces",
			"Vanguard/Variant_Combat/UI",
			"Vanguard/Variant_SideScrolling",
			"Vanguard/Variant_SideScrolling/AI",
			"Vanguard/Variant_SideScrolling/Gameplay",
			"Vanguard/Variant_SideScrolling/Interfaces",
			"Vanguard/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
