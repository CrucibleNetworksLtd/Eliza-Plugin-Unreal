// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Misc/EngineVersion.h"
#include "PlatformHttp.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/EngineVersionComparison.h"
#include "Eliza.h"
#include "Misc/ConfigCacheIni.h"
#include "ElizaHttpHelperLibrary.generated.h"

UCLASS()
class ELIZA_API UElizaHttpHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION()
	static FString GetElizaStarterUrl() {
		FString Location = "http://localhost:3000"; //default
		GConfig->GetString(TEXT("/Script/ElizaEditor.ElizaPluginSettings"), TEXT("ElizaHttpServerLocation"), Location, GGameIni);
		return Location;
	}

	template<typename T>
	inline static TSharedRef<IHttpRequest, ESPMode::ThreadSafe> ExecuteHttpRequest(T* FunctionBindObject, void(T::* FunctionBindFunction)(FHttpRequestPtr, FHttpResponsePtr, bool), const FString& URL, const FString& Verb = TEXT("GET"), const float& Timeout = 60.0F, const TArray<TPair<FString, FString>>& Headers = TArray<TPair<FString, FString>>(), const FString& Content = FString(), const bool ProcessRequestInstantly = true)
	{
		static_assert(std::is_base_of<UObject, T>::value, "T not derived from UObject");

		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

		/*if (GEngine && static_cast<UObject*>(FunctionBindObject)) {
			UObject* WorldContextObject = static_cast<UObject*>(FunctionBindObject);
			if (WorldContextObject) {
				UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
				if (
					World && //get the world
					World->GetGameInstance()) { //if we actually got a world, get the game instance
					UElizaEVMServerSubsystem* ElizaSubsystem = World->GetGameInstance()->GetSubsystem<UElizaEVMServerSubsystem>();
					if (ElizaSubsystem) {
						ElizaSubsystem->ActiveRequests.Add(HttpRequest);
					}
				};
			}
		}*/

		if (FunctionBindFunction && FunctionBindObject) {
			HttpRequest->OnProcessRequestComplete().BindUObject(FunctionBindObject, FunctionBindFunction);
		}

		if (URL.IsEmpty()) {
			UE_LOG(LogElizaHttp, Warning, TEXT("Tried to ExecuteHttpRequest but URL was empty"));
			return HttpRequest;
		}

		HttpRequest->SetURL(URL);
		HttpRequest->SetVerb(Verb);
		HttpRequest->SetTimeout(Timeout);
#if UE_VERSION_NEWER_THAN(5, 4, 0)
		HttpRequest->SetActivityTimeout(Timeout);
#endif
		//Handle headers and logging of the headers
		FString HeaderLogText;
		if (Headers.Num() > 0) {
			HeaderLogText = "\nAdditional headers:\n";
			for (int i = 0; i < Headers.Num(); i++) {
				HttpRequest->SetHeader(Headers[i].Key, Headers[i].Value);
				HeaderLogText.Append(Headers[i].Key + ": " + Headers[i].Value + "\n");
			}
		}

		if (Content.Len() > 0 && HttpRequest->GetHeader("Content-Type").Len() > 0) {
			HttpRequest->SetContentAsString(Content);
		}

		if (ProcessRequestInstantly) {
			UE_LOG(LogElizaHttp, Display, TEXT("Sent %s request to \"%s\", timing out in %f %s \n%s"), *Verb, *URL, Timeout, *HeaderLogText, *Content);
			HttpRequest->ProcessRequest();
		}
		else {
			UE_LOG(LogElizaHttp, Display, TEXT("%s request ready to send to \"%s\", will time out in %f %s \n%s"), *Verb, *URL, Timeout, *HeaderLogText, *Content);
		}

		return HttpRequest;
	};
};
