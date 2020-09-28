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

        // �� include ��ӵ� PublicIncludePaths
        // PublicAdditionalLibraries ������Ӷ��
        // --------------------------------------
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));

        // dll �ļ��ط�ʽ
        // ������Ϊ������Ҫ�Ӱ�
        // --------------------
        string dllPath_LabTest_dll = Path.Combine(ThirdPartyPath, "lib", "openctm.dll");
        PublicDelayLoadDLLs.Add(dllPath_LabTest_dll);
        RuntimeDependencies.Add(new RuntimeDependency(dllPath_LabTest_dll));
    }

    /// <summary>
    /// ThirdParty �ļ��е�����·��
    /// </summary>
    public string ThirdPartyPath
    {
        get
        {
            // �����ַ�Ƕ��٣�
            // ---------------
            string notFullPath = Path.Combine(ModuleDirectory, "../../ThirdParty");

            // ����ĵ��Է���
            // ���빤�̣������ɴ��ڲ鿴
            // C:\Users\zhang\Documents\Unreal Projects\UELabTest\Source\UELabTest\../../ThirdParty
            // ------------------------
            Console.WriteLine(String.Format("notFullPath is {0}", notFullPath));

            // ���� Path.GetFullPath
            // ��������ǰ�../../�������
            // --------------------------
            string fullPath = Path.GetFullPath(notFullPath);
            Console.WriteLine(String.Format("fullPath is {0}", fullPath));

            return fullPath;
        }
    }
}
