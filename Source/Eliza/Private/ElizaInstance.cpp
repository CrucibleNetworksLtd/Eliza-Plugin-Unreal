#include "ElizaInstance.h"

UElizaInstance* UElizaInstance::CreateElizaInstance(FString LocationURL, EElizaAPIType APIType, FString FleekAPIKey, FString FleekAgentId) {
	UElizaInstance* ReturnObject = NewObject<UElizaInstance>();
	ReturnObject->ElizaInstance.APIType = APIType;
	ReturnObject->ElizaInstance.LocationURL = LocationURL;
	ReturnObject->ElizaInstance.FleekAPIKey = FleekAPIKey;
	ReturnObject->ElizaInstance.FleekAgentId = FleekAgentId;
	return ReturnObject;
}

FString UElizaInstance::GetAPIUrl()
{
	if (this->ElizaInstance.APIType == EElizaAPIType::GenericEliza) {
		return this->ElizaInstance.LocationURL + "/";
	}
	if (this->ElizaInstance.APIType == EElizaAPIType::Fleek) {
		return "https://api.fleek.xyz/api/v1/ai-agents/" + this->ElizaInstance.FleekAgentId + "/api/";
	}
	checkNoEntry();
	return FString(); //this should never happen
}

TArray<TPair<FString, FString>> UElizaInstance::RequiredHeaders()
{
	TArray<TPair<FString, FString>> RequiredHeaders;
	if (this->ElizaInstance.APIType == EElizaAPIType::Fleek) {
		RequiredHeaders.Add(TPair<FString, FString>{"x-api-key", "" + this->ElizaInstance.FleekAPIKey});
	}
	return RequiredHeaders;
}
