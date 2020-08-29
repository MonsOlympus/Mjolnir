// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Mjolnir : TargetRules
{
	public Mjolnir(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        //Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "Mjolnir" });

        //if (bBuildEditor) 
    }
}