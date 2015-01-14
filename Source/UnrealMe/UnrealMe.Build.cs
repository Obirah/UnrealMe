// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealMe : ModuleRules
{
	public UnrealMe(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "K4WLib" });
		DynamicallyLoadedModuleNames.AddRange(new string[] {"K4WLib"});
	}
}
