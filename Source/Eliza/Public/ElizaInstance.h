// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "ElizaInstance.generated.h"

UENUM(BlueprintType)
enum class EElizaAPIType : uint8
{
	GenericEliza,
	Fleek
};

USTRUCT(BlueprintType)
struct ELIZA_API FElizaInstanceStruct
{
	GENERATED_BODY()

	//The URL of where the Eliza instance is. Usually "http://localhost:3000" if you're self hosting. Not used if you specify Fleek in the APIType.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	FString LocationURL;

	//If this is a normal Eliza instance or a Fleek Eliza instance.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	EElizaAPIType APIType;

	//A Fleek API key. NOT a bearer token!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	FString FleekAPIKey;
	
	//The Fleek agent ID. Not the Fleek Eliza agent ID!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	FString FleekAgentId;
};

UCLASS(BlueprintType, CollapseCategories)
class ELIZA_API UElizaInstance : public UObject
{

	GENERATED_BODY()

public:
	UElizaInstance() {};

	UElizaInstance(FElizaInstanceStruct ElizaInstance);

	UFUNCTION(BlueprintPure, Category = "Eliza|Instance")
	static UElizaInstance* CreateElizaInstance(FString LocationURL, EElizaAPIType APIType, FString FleekAPIKey, FString FleekAgentId);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Eliza", meta=(ShowInnerProperties))
	FElizaInstanceStruct ElizaInstance;

	UFUNCTION()
	FString GetAPIUrl();

	TArray<TPair<FString, FString>> RequiredHeaders();

};