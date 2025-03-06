// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "AgentDetails.h"
#include "ElizaInstance.h"
#include "GetAgentCharacter.generated.h"


UCLASS()
class ELIZA_API UGetAgentCharacter : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Given an AgentId, returns details about a the character that agent is representing from its character file.
	 * @param AgentId The ID string of the agent to get the character data of.
	 * @param ElizaInstance The Eliza instance that we should be communicating with. This can either be a ElizaInstance game asset created in the content browser, or created on the fly with the CreateElizaInstance method.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", KeyWords = "Eliza Character Agent"), Category = "Eliza")
	static UGetAgentCharacter* GetAgentCharacter(FString AgentId, UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetAgentCharacterCompleted, bool, Success, FAgentDetailsCharacter, Character);

	UPROPERTY(BlueprintAssignable)
	FOnGetAgentCharacterCompleted OnGetAgentCharacterCompleted;
private:
	void GetAgentCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FString AgentId;

	UElizaInstance* ElizaInstance;
};
