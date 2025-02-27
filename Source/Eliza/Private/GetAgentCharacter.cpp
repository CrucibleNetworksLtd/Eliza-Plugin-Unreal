// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "GetAgentCharacter.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UGetAgentCharacter* UGetAgentCharacter::GetAgentCharacter(FString AgentId)
{
	UGetAgentCharacter* BlueprintNode = NewObject<UGetAgentCharacter>();
	BlueprintNode->AgentId = AgentId;
	return BlueprintNode;
}

void UGetAgentCharacter::Activate()
{
	FString requestURL = UElizaHttpHelperLibrary::GetElizaStarterUrl() + "/Agents/" + AgentId;
	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});
	
	UElizaHttpHelperLibrary::ExecuteHttpRequest<UGetAgentCharacter>(
		this,
		&UGetAgentCharacter::GetAgentCharacter_HttpRequestComplete,
		requestURL
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
