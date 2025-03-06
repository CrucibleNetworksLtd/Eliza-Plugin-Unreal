// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "AgentDetails.h"
#include "ElizaInstance.h"
#include "CreateNewCharacter.generated.h"


UCLASS()
class ELIZA_API UCreateNewCharacter : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates a new Eliza agent on the server with the given character. The GUID of the agent is returned as AgentID.
	 * @param AgentCharacter The character struct that the agent will use.
	 * @param ElizaInstance The Eliza instance that we should be communicating with. This can either be a ElizaInstance game asset created in the content browser, or created on the fly with the CreateElizaInstance method.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", KeyWords = "Eliza Agent CreateCharacter"), Category = "Eliza")
	static UCreateNewCharacter* CreateNewCharacter(FAgentDetailsCharacter AgentCharacter, UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCreateNewCharacterCompleted, bool, Success, FString, AgentID);

	UPROPERTY(BlueprintAssignable)
	FOnCreateNewCharacterCompleted OnCreateNewCharacterCompleted;
private:
	void CreateNewCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FAgentDetailsCharacter AgentCharacter;

	UElizaInstance* ElizaInstance;
};
