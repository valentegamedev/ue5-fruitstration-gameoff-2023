// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Fruitstration : ModuleRules
{
	public Fruitstration(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] {"GeometryScriptingCore"});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "GeometryFramework", "FCTween"});
	}
}
