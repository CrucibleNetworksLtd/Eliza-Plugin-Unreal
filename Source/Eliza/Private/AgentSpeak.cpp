// Copyright Crucible Networks Ltd 2025. All Rights Reserved.


#include "AgentSpeak.h"
#include "HttpService/ElizaHttpHelperLibrary.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UAgentSpeak* UAgentSpeak::AgentSpeak(FString _AgentId, FString _Message, UElizaInstance* _ElizaInstance)
{
	UAgentSpeak* BlueprintNode = NewObject<UAgentSpeak>();
	BlueprintNode->AgentId = _AgentId;
	BlueprintNode->Message = _Message;
	BlueprintNode->ElizaInstance = _ElizaInstance;
	return BlueprintNode;
}

void UAgentSpeak::Activate()
{
	FString requestURL = ElizaInstance->GetAPIUrl() + AgentId + "/tts";

	TArray<TPair<FString, FString>> Headers;
	Headers.Add(TPair<FString, FString>{"content-type", "application/json"});

	Headers.Append(ElizaInstance->RequiredHeaders());
	

	TSharedPtr<FJsonObject> BodyContentJsonObject = MakeShareable(new FJsonObject);
	BodyContentJsonObject->SetStringField(TEXT("text"), Message);
	FString JsonOutput;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&JsonOutput);
	FJsonSerializer::Serialize(BodyContentJsonObject.ToSharedRef(), Writer);

	UElizaHttpHelperLibrary::ExecuteHttpRequest<UAgentSpeak>(
		this,
		&UAgentSpeak::AgentSpeak_HttpRequestComplete,
		requestURL,
		"POST",
		60.0F,
		Headers,
		JsonOutput
	);
}

void UAgentSpeak::AgentSpeak_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	// [{"user":"eliza","text":"Loud and clear! My circuits are humming along, ready for whatever test you throw my way. If you need a debugging buddy or just someone to share the testing woes with, I'm here. How's everything looking on your end?","action":"NONE"}]
	if (bSucceeded) {
		TSharedPtr<FJsonValue> JsonValue;
		FString ResponseString = HttpResponse->GetContentAsString();
		UE_LOG(LogEliza, Display, TEXT("Message Agent response: %s"), *ResponseString);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(Reader, JsonValue)) {
			if (JsonValue->AsObject()->HasField(TEXT("error"))) {
				FString ErrorDetails = JsonValue->AsObject()->GetStringField(TEXT("details"));
				UE_LOG(LogEliza, Error, TEXT("Eliza error: %s"), *HttpResponse->GetContentAsString());
				if (ErrorDetails.Contains("api")) {
					UE_LOG(LogEliza, Error, TEXT("Error doing TTS. Your Eliza instance is missing its ElevenLabs API key."));
				}
			}


			//OnAgentSpeakCompleted.Broadcast(true, User, Text, Action);
			return;
		}
	}

	OnAgentSpeakCompleted.Broadcast(false, FString(), FString(), FString());
	return;
}
