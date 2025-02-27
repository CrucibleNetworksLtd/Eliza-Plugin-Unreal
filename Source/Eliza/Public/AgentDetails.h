// Copyright Crucible Networks Ltd 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AgentDetails.generated.h"

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacterStyle
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|style", EditAnywhere, BlueprintReadWrite)
  TArray<FString> all;

  UPROPERTY(Category="JSON|AgentDetails|style", EditAnywhere, BlueprintReadWrite)
  TArray<FString> chat;

  UPROPERTY(Category="JSON|AgentDetails|style", EditAnywhere, BlueprintReadWrite)
  TArray<FString> post;

  FAgentDetailsCharacterStyle() {};

  FAgentDetailsCharacterStyle( TArray<FString> _all, TArray<FString> _chat, TArray<FString> _post ){

    all = _all;
    chat = _chat;
    post = _post;
  
  }
  
};

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacterMessageExamplesContent
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|content", EditAnywhere, BlueprintReadWrite)
  FString text;

  FAgentDetailsCharacterMessageExamplesContent() {};

  FAgentDetailsCharacterMessageExamplesContent( FString _text ){

    text = _text;
  
  }
  
};

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacterMessageExample
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|0", EditAnywhere, BlueprintReadWrite)
  FString user;

  UPROPERTY(Category="JSON|AgentDetails|0", EditAnywhere, BlueprintReadWrite)
  FAgentDetailsCharacterMessageExamplesContent content;

  FAgentDetailsCharacterMessageExample() {};

  FAgentDetailsCharacterMessageExample( FString _user, FAgentDetailsCharacterMessageExamplesContent _content ){

    user = _user;
    content = _content;
  
  }
  
};

USTRUCT(Category = "JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacterMessageCombination
{

    GENERATED_BODY()

    UPROPERTY(Category = "JSON|AgentDetails|0", EditAnywhere, BlueprintReadWrite)
    TArray<FAgentDetailsCharacterMessageExample> Combination;

    FAgentDetailsCharacterMessageCombination() {};

    FAgentDetailsCharacterMessageCombination(TArray<FAgentDetailsCharacterMessageExample> _combination) {

        Combination = _combination;

    }

};

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacterSettingsVoice
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|voice", EditAnywhere, BlueprintReadWrite)
  FString model;

  FAgentDetailsCharacterSettingsVoice() {};

  FAgentDetailsCharacterSettingsVoice( FString _model ){

    model = _model;
  
  }
  
};

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacterSetting
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|setting", EditAnywhere, BlueprintReadWrite)
  FAgentDetailsCharacterSettingsVoice voice;

  FAgentDetailsCharacterSetting() {};

  FAgentDetailsCharacterSetting( FAgentDetailsCharacterSettingsVoice _voice ){

    voice = _voice;
  
  }
  
};

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetailsCharacter
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FString name;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FString username;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FString modelProvider;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FAgentDetailsCharacterSetting settings;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FString system;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  TArray<FString> bio;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  TArray<FString> lore;

  UPROPERTY(Category = "JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  TArray<FAgentDetailsCharacterMessageCombination> messageExamples;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  TArray<FString> postExamples;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  TArray<FString> topics;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FAgentDetailsCharacterStyle style;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  TArray<FString> adjectives;

  UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  FString id;

  FAgentDetailsCharacter() {};

  FAgentDetailsCharacter( FString _name, FString _username, FString _modelProvider, FAgentDetailsCharacterSetting _settings, FString _system, TArray<FString> _bio, TArray<FString> _lore, TArray<FAgentDetailsCharacterMessageCombination> _messageExamples, TArray<FString> _postExamples, TArray<FString> _topics, FAgentDetailsCharacterStyle _style, TArray<FString> _adjectives, FString _id ){

    name = _name;
    username = _username;
    modelProvider = _modelProvider;
    settings = _settings;
    system = _system;
    bio = _bio;
    lore = _lore;
    messageExamples = _messageExamples;
    postExamples = _postExamples;
    topics = _topics;
    style = _style;
    adjectives = _adjectives;
    id = _id;
  
  }
  
  FAgentDetailsCharacter(FString _json_) {
      FAgentDetailsCharacter CharacterStruct;

      FJsonObjectConverter::JsonObjectStringToUStruct<FAgentDetailsCharacter>(_json_, &CharacterStruct, 0, 0);

      TSharedPtr<FJsonObject> JsonObject;
      TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(_json_);
      if (FJsonSerializer::Deserialize(JsonReader, JsonObject) || JsonObject.IsValid())
      {
          auto MessageExamplesArray = JsonObject->GetArrayField(TEXT("messageExamples"));
          for (int i = 0; i < MessageExamplesArray.Num(); i++) {

              auto UserMessageExampleJson = MessageExamplesArray[i]->AsArray()[0]->AsObject();
              FAgentDetailsCharacterMessageExample UserMessage;
              UserMessage.user = UserMessageExampleJson->GetStringField(TEXT("user"));
              UserMessage.content = UserMessageExampleJson->GetObjectField(TEXT("content"))->GetStringField(TEXT("text"));

              auto AIMessageExampleJson = MessageExamplesArray[i]->AsArray()[1]->AsObject();
              FAgentDetailsCharacterMessageExample AIMessage;
              AIMessage.user = AIMessageExampleJson->GetStringField(TEXT("user"));
              AIMessage.content = AIMessageExampleJson->GetObjectField(TEXT("content"))->GetStringField(TEXT("text"));
              TArray<FAgentDetailsCharacterMessageExample> Example;
              Example.Add(UserMessage);
              Example.Add(AIMessage);
              messageExamples.Add(FAgentDetailsCharacterMessageCombination(Example));
          }
      }
  }
};

USTRUCT(Category="JSON|AgentDetails", BlueprintType)
struct FAgentDetails
{

  GENERATED_BODY()

  UPROPERTY(Category="JSON|AgentDetails|AgentDetail", EditAnywhere, BlueprintReadWrite)
  FString id;

  UPROPERTY(Category="JSON|AgentDetails|AgentDetail", EditAnywhere, BlueprintReadWrite)
  FAgentDetailsCharacter character;

  FAgentDetails() {};

  FAgentDetails( FString _id, FAgentDetailsCharacter _character ){

    id = _id;
    character = _character;
  
  }
  
  /* Don't Forget to setup your project
  Add #include "Runtime/JsonUtilities/Public/JsonObjectConverter.h" in 
  file with this structs.
  Also you need add "Json", "JsonUtilities" in Build.cs */

  TSharedPtr<FJsonObject> FAgentDetailsToJson() {
      return FJsonObjectConverter::UStructToJsonObject<FAgentDetails>(*this);
  }

  FAgentDetails(FString _json_){
    FAgentDetails _tmpAgentDetails;
    
		FJsonObjectConverter::JsonObjectStringToUStruct<FAgentDetails>(
		_json_,
		&_tmpAgentDetails,
    0, 0);
    
    id = _tmpAgentDetails.id;
    character = _tmpAgentDetails.character;

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(_json_);
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) || JsonObject.IsValid())
    {
        auto MessageExamplesArray = JsonObject->GetObjectField(TEXT("character"))->GetArrayField(TEXT("messageExamples"));
        for (int i = 0; i < MessageExamplesArray.Num(); i++) {
           
            auto UserMessageExampleJson = MessageExamplesArray[i]->AsArray()[0]->AsObject();
            FAgentDetailsCharacterMessageExample UserMessage;
            UserMessage.user = UserMessageExampleJson->GetStringField(TEXT("user"));
            UserMessage.content = UserMessageExampleJson->GetObjectField(TEXT("content"))->GetStringField(TEXT("text"));

            auto AIMessageExampleJson = MessageExamplesArray[i]->AsArray()[1]->AsObject();
            FAgentDetailsCharacterMessageExample AIMessage;
            AIMessage.user = AIMessageExampleJson->GetStringField(TEXT("user"));
            AIMessage.content = AIMessageExampleJson->GetObjectField(TEXT("content"))->GetStringField(TEXT("text"));
            TArray<FAgentDetailsCharacterMessageExample> Example;
            Example.Add(UserMessage);
            Example.Add(AIMessage);
            character.messageExamples.Add(FAgentDetailsCharacterMessageCombination(Example));
        }
    }
  }
  
};


UCLASS()
class UAgentDetailsHelperLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category = "Eliza")
    static FAgentDetailsCharacter CharacterFromJsonString(FString JsonString)
    {
        return FAgentDetailsCharacter(JsonString);
    }
};