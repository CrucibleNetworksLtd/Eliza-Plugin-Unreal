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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	FString LocationURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	EElizaAPIType APIType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eliza|Instance")
	FString FleekAPIKey;

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