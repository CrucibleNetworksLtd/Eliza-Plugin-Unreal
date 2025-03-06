// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "CreateNewCharacter.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/Guid.h"

UCreateNewCharacter* UCreateNewCharacter::CreateNewCharacter(FAgentDetailsCharacter _AgentCharacter, UElizaInstance* _ElizaInstance)
{
	UCreateNewCharacter* BlueprintNode = NewObject<UCreateNewCharacter>();
	BlueprintNode->AgentCharacter = _AgentCharacter;
	BlueprintNode->ElizaInstance = _ElizaInstance;
	return BlueprintNode;
}

void UCreateNewCharacter::Activate()
{
	if (!ElizaInstance) { //Prevent continuing if we don't have an eliza instance.
		UE_LOG(LogEliza, Error, TEXT("You must supply an Eliza Instance to comminicate with. You can create them in the content browser, or use the CreateElizaInstance function."));
		return;
	}

	FString AgentID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);

	FString requestURL = ElizaInstance->GetAPIUrl() + "Agents/" + AgentID + "/set";

	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});

	Headers.Append(ElizaInstance->RequiredHeaders());

	FString JsonOutput;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&JsonOutput);
	auto CharacterJson = AgentCharacter.FAgentDetailsCharacterToJson().ToSharedRef();
	CharacterJson->SetArrayField(TEXT("clients"), TArray<TSharedPtr<FJsonValue>>());
	CharacterJson->SetArrayField(TEXT("plugins"), TArray<TSharedPtr<FJsonValue>>());
	CharacterJson->SetStringField(TEXT("id"), AgentID);
	FJsonSerializer::Serialize(CharacterJson, Writer);
	
	UE_LOG(LogTemp, Display, TEXT("%s"), *JsonOutput);

	UElizaHttpHelperLibrary::ExecuteHttpRequest<UCreateNewCharacter>(
		this,
		&UCreateNewCharacter::CreateNewCharacter_HttpRequestComplete,
		requestURL,
		"POST",
		60.0F,
		Headers,
		JsonOutput
	);
}

void UCreateNewCharacter::CreateNewCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded) {
		TSharedPtr<FJsonValue> JsonValue;
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogEliza, Display, TEXT("Set Agent Character response: %s"), *ResponseString);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			OnCreateNewCharacterCompleted.Broadcast(true, JsonValue->AsObject()->GetStringField(TEXT("id")));

			return;
		}
	}

	OnCreateNewCharacterCompleted.Broadcast(false, "");
	return;
}
