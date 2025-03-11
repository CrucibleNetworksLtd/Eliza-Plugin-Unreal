// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "EditExistingCharacter.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/Guid.h"

UEditExistingCharacter* UEditExistingCharacter::EditExistingCharacter(FString _AgentId, FAgentDetailsCharacter _AgentCharacter, UElizaInstance* _ElizaInstance)
{
	UEditExistingCharacter* BlueprintNode = NewObject<UEditExistingCharacter>();
	BlueprintNode->AgentCharacter = _AgentCharacter;
	BlueprintNode->ElizaInstance = _ElizaInstance;
	BlueprintNode->AgentId = _AgentId;
	return BlueprintNode;
}

void UEditExistingCharacter::Activate()
{
	if (!ElizaInstance) { //Prevent continuing if we don't have an eliza instance.
		UE_LOG(LogEliza, Error, TEXT("You must supply an Eliza Instance to comminicate with. You can create them in the content browser, or use the CreateElizaInstance function."));
		return;
	}

	if (AgentId.IsEmpty()) {
		UE_LOG(LogEliza, Error, TEXT("You must supply the AgentId of the character you want to edit on the eliza server."));
		return;
	}

	AgentCharacter.id = AgentId;

	FString requestURL = ElizaInstance->GetAPIUrl() + "Agents/" + AgentId + "/set";

	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});

	Headers.Append(ElizaInstance->RequiredHeaders());
	

	FString JsonOutput;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&JsonOutput);
	auto CharacterJson = AgentCharacter.FAgentDetailsCharacterToJson().ToSharedRef();
	CharacterJson->SetArrayField(TEXT("clients"), TArray<TSharedPtr<FJsonValue>>());
	CharacterJson->SetArrayField(TEXT("plugins"), TArray<TSharedPtr<FJsonValue>>());
	CharacterJson->SetStringField(TEXT("id"), AgentId);
	FJsonSerializer::Serialize(CharacterJson, Writer);
	
	UE_LOG(LogTemp, Display, TEXT("%s"), *JsonOutput);

	UElizaHttpHelperLibrary::ExecuteHttpRequest<UEditExistingCharacter>(
		this,
		&UEditExistingCharacter::EditExistingCharacter_HttpRequestComplete,
		requestURL,
		"POST",
		60.0F,
		Headers,
		JsonOutput
	);
}

void UEditExistingCharacter::EditExistingCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded) {
		TSharedPtr<FJsonValue> JsonValue;
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogEliza, Display, TEXT("Set Agent Character response: %s"), *ResponseString);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			OnEditExistingCharacterCompleted.Broadcast(true);

			return;
		}
	}

	OnEditExistingCharacterCompleted.Broadcast(false);
	return;
}
