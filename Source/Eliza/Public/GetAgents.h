// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "ElizaInstance.h"
#include "GetAgents.generated.h"

USTRUCT(BlueprintType)
struct FAgentInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent")
	FString AgentId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent")
	FString Name;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Agent")
	//TArray<FString> Clients;

	FAgentInfo() {};

	FAgentInfo(FString _AgentId, FString _Name/*, TArray<FString> _Clients*/) {
		AgentId = _AgentId;
		Name = _Name;
		//Clients = _Clients;
	};
};

UCLASS()
class ELIZA_API UGetAgents : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Returns an array of all the agents running in your Eliza instance.
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Eliza")
	static UGetAgents* GetAgents(UElizaInstance* ElizaInstance);

	virtual void Activate() override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetAgentsCompleted, bool, Success, const TArray<FAgentInfo>&, Agents);

	UPROPERTY(BlueprintAssignable)
	FOnGetAgentsCompleted OnGetAgentsCompleted;
private:
	void GetAgents_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	FString AgentId, Message;

	UElizaInstance* ElizaInstance;
};
