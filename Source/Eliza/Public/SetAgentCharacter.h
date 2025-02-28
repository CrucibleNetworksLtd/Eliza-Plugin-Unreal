// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "AgentDetails.h"
#include "ElizaInstance.h"
#include "SetAgentCharacter.generated.h"


UCLASS()
class ELIZA_API USetAgentCharacter : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	//If CreateNew is set to false, then it will only try to update an existing one based on the given ID. If CreateNew is true, it will create a new agent based on the given ID
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Eliza")
	static USetAgentCharacter* SetAgentCharacter(FAgentDetailsCharacter AgentCharacter, bool CreateNew, UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSetAgentCharacterCompleted, bool, Success, FString, AgentID);

	UPROPERTY(BlueprintAssignable)
	FOnSetAgentCharacterCompleted OnSetAgentCharacterCompleted;
private:
	void SetAgentCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FAgentDetailsCharacter AgentCharacter;

	bool CreateNew;

	UElizaInstance* ElizaInstance;
};
