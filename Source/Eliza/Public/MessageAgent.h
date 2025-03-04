// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "ElizaInstance.h"
#include "MessageAgent.generated.h"


UCLASS()
class ELIZA_API UMessageAgent : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Sends a message to an agent.
	 * @param AgentId The ID string of the agent to send the message to.
	 * @param Message The message to send to the agent.
	 * @param Speaker The user that spoke the message you're sending. Leave blank to be the default user.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Eliza")
	static UMessageAgent* MessageAgent(FString AgentId, FString Message, FString Speaker, UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnMessageAgentCompleted, bool, Success, FString, User, FString, Text, FString, Action);

	UPROPERTY(BlueprintAssignable)
	FOnMessageAgentCompleted OnMessageAgentCompleted;
private:
	void MessageAgent_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FString AgentId, Message, Speaker;

	UElizaInstance* ElizaInstance;
};
