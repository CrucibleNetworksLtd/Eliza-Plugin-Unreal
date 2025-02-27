// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#pragma once
#include "Engine/EngineTypes.h"
#include "ElizaPluginSettings.generated.h"

UCLASS(config = Game, defaultconfig)
class UElizaPluginSettings : public UObject
{
	GENERATED_BODY()

public:

	//Where is the Eliza starter webserver located? Default is localhost:3000.
	UPROPERTY(config, EditAnywhere, Category = "General")
	FString ElizaHttpServerLocation = "http://localhost:3000";
};