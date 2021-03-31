// Fill out your copyright notice in the Description page of Project Settings.

#include "C_ScriptDirector.h"
#include <Json.h>


void restore_script(TSharedRef<FJsonObject> json_object)
{
     //FString ExampleString = json_object->GetStringField("exampleString");
     //auto json_parsed_ptr = json_object.Get();
     TArray<FString> keys;
     json_object->Values.GetKeys(keys);
     for (auto& key : keys)
     {
         ULOG(Warning, "Key: %s", *key);
     }
}

 void ParseExample()
 {
     ULOG(Log, "Enter the ParseExample()");

     const FString SCRIPT_FILENAME("script.json");
     const FString script_full_path = FPaths::ProjectDir().Append(SCRIPT_FILENAME);

     FString json_raw;
     if (FFileHelper::LoadFileToString(json_raw, *script_full_path) == false)
     {
         ULOG(Error, "Failed LoadJson : %s", *script_full_path);
     }

     TSharedPtr<FJsonObject> json_object;
     TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(json_raw);

     if (FJsonSerializer::Deserialize(json_reader, json_object) && json_object.IsValid())
     {
         ULOG(Log, "Deserialized");
         restore_script(json_object.ToSharedRef());
     }
     else
     {
         ULOG(Error, "Unable to deserialize script from file \"%s\"", *script_full_path);
     }
 }

AC_ScriptDirector::AC_ScriptDirector()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    ParseExample();
}

void AC_ScriptDirector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AC_ScriptDirector::BeginPlay()
{
    // Called when the game starts or when spawned
    Super::BeginPlay();
    
}

void AC_ScriptDirector::read_story_from_file(const FString story_filename)
{

}

void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard *interact_card, CardType &card_type, TArray<FDialogUnit> &dialog, TArray<FText> &Answers)
{
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, TArray<FDialogUnit> &dialog, ActionWithCard &action_with_card)
{
}

