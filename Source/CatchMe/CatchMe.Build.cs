// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CatchMe : ModuleRules
{
	public CatchMe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

        PublicIncludePaths.AddRange(new string[]
        {
            "CatchMe", // 이게 없으면 헤더파일 인클루드시 경로를 찾지 못함.
		});
    }


}
