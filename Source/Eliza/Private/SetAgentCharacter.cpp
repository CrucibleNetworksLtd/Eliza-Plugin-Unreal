// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "SetAgentCharacter.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

USetAgentCharacter* USetAgentCharacter::SetAgentCharacter(FAgentDetailsCharacter _AgentCharacter, bool _CreateNew, UElizaInstance* _ElizaInstance)
{
	USetAgentCharacter* BlueprintNode = NewObject<USetAgentCharacter>();
	BlueprintNode->AgentCharacter = _AgentCharacter;
	BlueprintNode->ElizaInstance = _ElizaInstance;
	BlueprintNode->CreateNew = _CreateNew;
	return BlueprintNode;
}

void USetAgentCharacter::Activate()
{
	if (!ElizaInstance) { //Prevent continuing if we don't have an eliza instance.
		UE_LOG(LogEliza, Error, TEXT("You must supply an Eliza Instance to comminicate with. You can create them in the content browser, or use the CreateElizaInstance function."));
		return;
	}

	FString AgentID;
	
	if (CreateNew) {
		AgentID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
	}
	else {
		AgentID = AgentCharacter.id; //Get the ID from the given data struct
	}

	FString requestURL = ElizaInstance->GetAPIUrl() + AgentID + "/set";

	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});

	Headers.Append(ElizaInstance->RequiredHeaders());
	

	FString JsonOutput;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&JsonOutput);
	auto CharacterJson = AgentCharacter.FAgentDetailsCharacterToJson().ToSharedRef();
	//CharacterJson->RemoveField("id");
	CharacterJson->SetArrayField("clients", TArray<TSharedPtr<FJsonValue>>());
	CharacterJson->SetArrayField("plugins", TArray<TSharedPtr<FJsonValue>>());
	CharacterJson->SetStringField("id", AgentID);
	FJsonSerializer::Serialize(CharacterJson, Writer);
	
	UE_LOG(LogTemp, Display, TEXT("%s"), *JsonOutput);

	UElizaHttpHelperLibrary::ExecuteHttpRequest<USetAgentCharacter>(
		this,
		&USetAgentCharacter::SetAgentCharacter_HttpRequestComplete,
		requestURL,
		"POST",
		60.0F,
		Headers,
		JsonOutput
	);
}

void USetAgentCharacter::SetAgentCharacter_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded) {
		TSharedPtr<FJsonValue> JsonValue;
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogEliza, Display, TEXT("Set Agent Character response: %s"), *ResponseString);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			OnSetAgentCharacterCompleted.Broadcast(true, JsonValue->AsObject()->GetStringField("id"));

			return;
		}
	}

	OnSetAgentCharacterCompleted.Broadcast(false, "");
	return;
}
