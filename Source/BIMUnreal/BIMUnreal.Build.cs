// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class BIMUnreal : ModuleRules
{
	public BIMUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        //PrivateDependencyModuleNames.AddRange(new string[] {  });
        PrivateDependencyModuleNames.AddRange(new string[] { "Json", "Http" });//, "JsonUtilities" 

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        // 将 include 添加到 PublicIncludePaths
        // PublicAdditionalLibraries 可以添加多个
        // --------------------------------------
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));

        // dll 的加载方式
        // 暂且认为两个都要加吧
        // --------------------
        string dllPath_LabTest_dll = Path.Combine(ThirdPartyPath, "lib", "openctm.dll");
        PublicDelayLoadDLLs.Add(dllPath_LabTest_dll);
        RuntimeDependencies.Add(new RuntimeDependency(dllPath_LabTest_dll));
    }

    /// <summary>
    /// ThirdParty 文件夹的所在路径
    /// </summary>
    public string ThirdPartyPath
    {
        get
        {
            // 这个地址是多少？
            // ---------------
            string notFullPath = Path.Combine(ModuleDirectory, "../../ThirdParty");

            // 这里的调试方法
            // 编译工程，从生成窗口查看
            // C:\Users\zhang\Documents\Unreal Projects\UELabTest\Source\UELabTest\../../ThirdParty
            // ------------------------
            Console.WriteLine(String.Format("notFullPath is {0}", notFullPath));

            // 调用 Path.GetFullPath
            // 这个方法是把../../计算出来
            // --------------------------
            string fullPath = Path.GetFullPath(notFullPath);
            Console.WriteLine(String.Format("fullPath is {0}", fullPath));

            return fullPath;
        }
    }
}
