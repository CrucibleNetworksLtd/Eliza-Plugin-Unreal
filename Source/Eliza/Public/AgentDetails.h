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

  UPROPERTY(Category = "JSON|AgentDetails|setting", EditAnywhere, BlueprintReadWrite)
  TMap<FString,FString> secrets;

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

  //Appears to be not used by anything, removing for now
  //UPROPERTY(Category="JSON|AgentDetails|character", EditAnywhere, BlueprintReadWrite)
  //FString system;

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

  FAgentDetailsCharacter( FString _name, FString _username, FString _modelProvider, FAgentDetailsCharacterSetting _settings, /*FString _system,*/ TArray<FString> _bio, TArray<FString> _lore, TArray<FAgentDetailsCharacterMessageCombination> _messageExamples, TArray<FString> _postExamples, TArray<FString> _topics, FAgentDetailsCharacterStyle _style, TArray<FString> _adjectives, FString _id) {

    name = _name;
    username = _username;
    modelProvider = _modelProvider;
    settings = _settings;
    //system = _system;
    bio = _bio;
    lore = _lore;
    messageExamples = _messageExamples;
    postExamples = _postExamples;
    topics = _topics;
    style = _style;
    adjectives = _adjectives;
    id = _id;
  
  }

  TSharedPtr<FJsonObject> FAgentDetailsCharacterToJson() {
      TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject<FAgentDetailsCharacter>(*this);

      TArray<TSharedPtr<FJsonValue>> MessageExamplesArray;
      for (auto Example : this->messageExamples) {
          TArray<TSharedPtr<FJsonValue>> ExampleArray;
          for (auto Message : Example.Combination) {
              auto MessageJson = FJsonObjectConverter::UStructToJsonObject<FAgentDetailsCharacterMessageExample>(Message);
              ExampleArray.Add(MakeShareable(new FJsonValueObject(MessageJson)));
          }
          if (ExampleArray.Num() > 0) {
              MessageExamplesArray.Add(MakeShareable(new FJsonValueArray(ExampleArray)));
          }
      }

      Json->SetArrayField(TEXT("messageExamples"), MessageExamplesArray);

      return Json;
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

//This is effective a hidden type, used only for deserialzing json returned from eliza to pull out the FAgentDetailsCharacter part.
USTRUCT()
struct FAgentDetails
{

  GENERATED_BODY()

  UPROPERTY()
  FString id;

  UPROPERTY()
  FAgentDetailsCharacter character;

  FAgentDetails() {};

  FAgentDetails( FString _id, FAgentDetailsCharacter _character ){

    id = _id;
    character = _character;
  
  }

  TSharedPtr<FJsonObject> FAgentDetailsToJson() {
      TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject<FAgentDetails>(*this);
      
      TArray<TSharedPtr<FJsonValue>> MessageExamplesArray;
      for (auto Example : this->character.messageExamples) {
          TArray<TSharedPtr<FJsonValue>> ExampleArray;
          for (auto Message : Example.Combination) {
              auto MessageJson = FJsonObjectConverter::UStructToJsonObject<FAgentDetailsCharacterMessageExample>(Message);
              ExampleArray.Add(MakeShareable(new FJsonValueObject(MessageJson)));
          }
          if (ExampleArray.Num() > 0) {
              MessageExamplesArray.Add(MakeShareable(new FJsonValueArray(ExampleArray)));
          }
      }

      Json->GetObjectField(TEXT("character"))->SetArrayField(TEXT("messageExamples"), MessageExamplesArray);

      return Json;
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