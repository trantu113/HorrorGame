// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyFirstHorrorGame : ModuleRules
{
	public MyFirstHorrorGame(ReadOnlyTargetRules Target) : base(Target)
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
			"MyFirstHorrorGame",
			"MyFirstHorrorGame/Variant_Platforming",
			"MyFirstHorrorGame/Variant_Platforming/Animation",
			"MyFirstHorrorGame/Variant_Combat",
			"MyFirstHorrorGame/Variant_Combat/AI",
			"MyFirstHorrorGame/Variant_Combat/Animation",
			"MyFirstHorrorGame/Variant_Combat/Gameplay",
			"MyFirstHorrorGame/Variant_Combat/Interfaces",
			"MyFirstHorrorGame/Variant_Combat/UI",
			"MyFirstHorrorGame/Variant_SideScrolling",
			"MyFirstHorrorGame/Variant_SideScrolling/AI",
			"MyFirstHorrorGame/Variant_SideScrolling/Gameplay",
			"MyFirstHorrorGame/Variant_SideScrolling/Interfaces",
			"MyFirstHorrorGame/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
