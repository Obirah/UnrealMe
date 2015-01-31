// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
using UnrealBuildTool;

public class K4WLib : ModuleRules
{
    public K4WLib(TargetInfo Target)
	{
		Type = ModuleType.External;
		
		/* Path to the Kinect V2 SDK read from environment variable */
        string SDKDIR = Utils.ResolveEnvironmentVariable("%KINECTSDK20_DIR%");
		/* Change path format from Windows backslashes to forward slashes */
        SDKDIR = SDKDIR.Replace("\\", "/");

		if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
		{
			/* Add the SDK's include folder to the Unreal include paths. */
            PublicIncludePaths.Add(SDKDIR+"inc/");
			
			/* Use the fitting plattform suffix */
            string PlatformPath =  (Target.Platform == UnrealTargetPlatform.Win64) ? "x64/" : "x86/";
			/* Path to the SDK's lib folder*/
            string LibPath = SDKDIR+"Lib/"+PlatformPath;
			
			/* Add the Kinect library path to Unreal's libarary paths */
            PublicLibraryPaths.Add(LibPath);
			/* Add the needed static library */
            PublicAdditionalLibraries.Add("Kinect20.lib");

			/* Path to the SDK's redistributable directory */
            string redistPath = SDKDIR + "Redist/";
			/* Add the VisualGestureBuilder dynamic library to Unreal's DLLs to be loaded. */
            PublicDelayLoadDLLs.AddRange(new string[] { redistPath+"VGB/"+PlatformPath+"Kinect20.VisualGestureBuilder.dll" });
		}
	}
}
