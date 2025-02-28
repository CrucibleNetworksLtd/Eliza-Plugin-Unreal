// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "ElizaInstance.h"
#include "AgentWhisper.generated.h"
/**
 * 
 */
UCLASS()
class ELIZA_API UAgentWhisper : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", AdvancedDisplay = "ElizaInstance"), Category = "Eliza")
	static UAgentWhisper* AgentWhisper(FString AgentId, FString Message, UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAgentWhisperCompleted, bool, Success, FString, User, FString, Text, FString, Action);

	UPROPERTY(BlueprintAssignable)
	FOnAgentWhisperCompleted OnAgentWhisperCompleted;
private:
	void AgentWhisper_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FString AgentId, Message;

	UElizaInstance* ElizaInstance;
};
