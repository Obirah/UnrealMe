// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;
using System.IO;

public class VRPNLib : ModuleRules
{
	private string ModulePath
    {
        get { return Path.GetDirectoryName( RulesCompiler.GetModuleFilename( this.GetType().Name ) ); }
    }
	
    public VRPNLib(TargetInfo Target)
	{
		Type = ModuleType.External;
		
		System.Diagnostics.Debug.WriteLine(ModulePath);

		/* Change path format from Windows backslashes to forward slashes */
        string VRPNDIR = ModulePath.Replace("\\", "/");

		if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
		{
			/* Add the VRPN include folder to the Unreal include paths. */
            PublicIncludePaths.Add(VRPNDIR + "/inc/");
			
			/* Use the fitting plattform suffix */
            //string PlatformPath =  (Target.Platform == UnrealTargetPlatform.Win64) ? "x64/" : "x86/";
			/* Path to the VRPN lib folder*/
            string LibPath = VRPNDIR + "/lib/";
			
			System.Diagnostics.Debug.WriteLine(LibPath);
			
			/* Add the VRPN library path to Unreal's libarary paths */
            PublicLibraryPaths.Add(LibPath);
			/* Add the needed static libraries */
            PublicAdditionalLibraries.Add("gpsnmealib.lib");
			PublicAdditionalLibraries.Add("quat.lib");
			PublicAdditionalLibraries.Add("vrpn.lib");		
		}
	}
}
