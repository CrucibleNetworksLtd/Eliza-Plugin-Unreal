// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "GetAgents.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UGetAgents* UGetAgents::GetAgents(UElizaInstance* _ElizaInstance)
{
	UGetAgents* BlueprintNode = NewObject<UGetAgents>();
	BlueprintNode->ElizaInstance = _ElizaInstance;
	return BlueprintNode;
}

void UGetAgents::Activate()
{
	if (!ElizaInstance) { //Prevent continuing if we don't have an eliza instance.
		UE_LOG(LogEliza, Error, TEXT("You must supply an Eliza Instance to comminicate with. You can create them in the content browser, or use the CreateElizaInstance function."));
		return;
	}

	FString requestURL = ElizaInstance->GetAPIUrl() + "Agents";

	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});

	Headers.Append(ElizaInstance->RequiredHeaders());

	UElizaHttpHelperLibrary::ExecuteHttpRequest<UGetAgents>(
		this,
		&UGetAgents::GetAgents_HttpRequestComplete,
		requestURL,
		"GET",
		60.0F,
		Headers
	);
}

void UGetAgents::GetAgents_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded) {
		TSharedPtr<FJsonValue> JsonValue;
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogEliza, Display, TEXT("Get Agents response: %s"), *ResponseString);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			TArray<FAgentInfo> Agents;
			auto AgentsArray = JsonValue->AsObject()->GetArrayField(TEXT("agents"));
			for (int i = 0; i < AgentsArray.Num(); i++) {
				auto Agent = AgentsArray[i]->AsObject();
				Agents.Add(FAgentInfo(Agent->GetStringField(TEXT("id")), Agent->GetStringField(TEXT("name"))));
			}
			OnGetAgentsCompleted.Broadcast(true, Agents);

			return;
		}
	}

	OnGetAgentsCompleted.Broadcast(false, TArray<FAgentInfo>());
	return;
}
