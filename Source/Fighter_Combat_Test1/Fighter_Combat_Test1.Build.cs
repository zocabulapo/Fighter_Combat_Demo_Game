// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Fighter_Combat_Test1 : ModuleRules
{
	public Fighter_Combat_Test1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore", "EnhancedInput" });
	}
}
