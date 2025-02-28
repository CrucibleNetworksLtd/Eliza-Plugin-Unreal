// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "GetAgentCharacter.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UGetAgentCharacter* UGetAgentCharacter::GetAgentCharacter(FString _AgentId, UElizaInstance* _ElizaInstance)
{
	UGetAgentCharacter* BlueprintNode = NewObject<UGetAgentCharacter>();
	BlueprintNode->AgentId = _AgentId;
	BlueprintNode->ElizaInstance = _ElizaInstance;
	return BlueprintNode;
}

void UGetAgentCharacter::Activate()
{
	if (!ElizaInstance) { //Prevent continuing if we don't have an eliza instance.
		UE_LOG(LogEliza, Error, TEXT("You must supply an Eliza Instance to comminicate with. You can create them in the content browser, or use the CreateElizaInstance function."));
		return;
	}

	if (AgentId.IsEmpty()) {
		UE_LOG(LogEliza, Error, TEXT("You must supply an Eliza AgentID to comminicate with. Try finding the ones in this Eliza instance with the GetAgents function."));
		return;
	}

	FString requestURL = ElizaInstance->GetAPIUrl() + "Agents/" + AgentId;

	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});

	Headers.Append(ElizaInstance->RequiredHeaders());
	
	UElizaHttpHelperLibrary::ExecuteHttpRequest<UGetAgentCharacter>(
		this,
		&UGetAgentCharacter::GetAgentCharacter_HttpRequestComplete,
		requestURL,
		"GET",
		60.0F,
		Headers
	);
}

void UGetAgentCharacter::GetAgentCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded) {
		TSharedPtr<FJsonValue> JsonValue;
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogEliza, Display, TEXT("Get Agent Character response: %s"), *ResponseString);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {

			FAgentDetails AgentDetails = FAgentDetails(HttpResponse->GetContentAsString());
			OnGetAgentCharacterCompleted.Broadcast(true, AgentDetails);

			return;
		}
	}

	OnGetAgentCharacterCompleted.Broadcast(false, FAgentDetails());
	return;
}
