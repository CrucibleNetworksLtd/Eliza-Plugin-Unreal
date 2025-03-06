// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "AgentDetails.h"
#include "ElizaInstance.h"
#include "EditExistingCharacter.generated.h"


UCLASS()
class ELIZA_API UEditExistingCharacter : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Edits an existing Eliza agent on the server, replacing its character with the given character. You must supply an a Character struct with the "id" being the same as the id of the character you want to edit on the eliza server.
	 * @param AgentId The AgentId of the agent you want to replace the character of.
	 * @param AgentCharacter The character struct that the agent will use. You must supply an a Character struct with the "id" being the same as the id of the character you want to edit on the eliza server.
	 * @param ElizaInstance The Eliza instance that we should be communicating with. This can either be a ElizaInstance game asset created in the content browser, or created on the fly with the CreateElizaInstance method.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", KeyWords = "Eliza Agent EditCharacter"), Category = "Eliza")
	static UEditExistingCharacter* EditExistingCharacter(FString AgentId, FAgentDetailsCharacter AgentCharacter, UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditExistingCharacterCompleted, bool, Success);

	UPROPERTY(BlueprintAssignable)
	FOnEditExistingCharacterCompleted OnEditExistingCharacterCompleted;
private:
	void EditExistingCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FAgentDetailsCharacter AgentCharacter;

	UElizaInstance* ElizaInstance;

	FString AgentId;
};
