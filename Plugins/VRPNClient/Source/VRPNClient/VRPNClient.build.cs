using UnrealBuildTool;
using System.IO;
 
public class VRPNClient : ModuleRules
{
	private string ModulePath
    {
        get { return Path.GetDirectoryName( RulesCompiler.GetModuleFilename( this.GetType().Name ) ); }
    }
 
    private string VRPNPath
    {
        get { return Path.GetFullPath( Path.Combine( ModulePath, "../vrpn_lib/" ) ); }
    }

    public VRPNClient(TargetInfo Target)
    {
        PrivateIncludePaths.AddRange(new string[] { "VRPNClient/Private" });
        PublicIncludePaths.AddRange(new string[] { "VRPNClient/Public", Path.Combine(VRPNPath, "Includes") });
		
		string tLibPath = Path.Combine(VRPNPath, "Libraries");
		PublicAdditionalLibraries.Add(Path.Combine(tLibPath, "vrpn.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(tLibPath, "vrpndll.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(tLibPath, "quat.lib"));
		PublicAdditionalLibraries.Add(Path.Combine(tLibPath, "gpsnmealib.lib"));
		
		Definitions.Add(string.Format( "WITH_VRPNCLIENT_BINDING={0}", 1 ) );
		
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core" });		
    }
}