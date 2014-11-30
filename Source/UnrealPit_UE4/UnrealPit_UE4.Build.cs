// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UnrealPit_UE4 : ModuleRules
{
	private string ModulePath
    {
        get { return Path.GetDirectoryName( RulesCompiler.GetModuleFilename( this.GetType().Name ) ); }
    }
	
	private string ThirdPartyPath
    {
        get {return Path.GetFullPath( Path.Combine(ModulePath, "../../Dependencies/" )); }
    }
	
	public UnrealPit_UE4(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore"});
		//DynamicallyLoadedModuleNames.AddRange(new string[] { "VRPNClient" });
		//PrivateDependencyModuleNames.AddRange(new string[] { "VRPNClient" });
		//PrivateIncludePathModuleNames.AddRange(new string[] { "VRPNClient" });
		//PublicIncludePaths.AddRange(new string[] {"VRPNClient/Public", "VRPNClient/Private"});
		//LoadVRPN(Target);
	}
	
	public bool LoadVRPN(TargetInfo Target)
    {
        bool isLibrarySupported = false;
 
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
 
            string LibrariesPath = Path.Combine("D:\\Eigene Dateien\\Dropbox\\Softwareentwicklung\\Unreal Projects\\UnrealPit_UE4\\Dependencies", "vrpn_lib", "Libraries");
 
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "vrpn.lib")); 
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "vrpndll.lib")); 
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "quat.lib")); 
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "gpsnmealib.lib"));		
        }
 
        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add( Path.Combine("D:\\Eigene Dateien\\Dropbox\\Softwareentwicklung\\Unreal Projects\\UnrealPit_UE4\\Dependencies", "vrpn_lib", "Includes" ) );
        }
 
        Definitions.Add(string.Format( "WITH_VRPN_BINDING={0}", isLibrarySupported ? 1 : 0 ) );
 
        return isLibrarySupported;
    }
}
