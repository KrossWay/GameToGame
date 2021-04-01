// Fill out your copyright notice in the Description page of Project Settings.

#include "C_ScriptDirector.h"
#include <Templates/SharedPointer.h>
#include <Json.h>


static const FString DIALOG_KEY("Dialog");
static const FString ANSWERS_KEY("Answers");
static const FString ACTIONS_KEY("Actions");

static const FString LEAF_TEXT_KEY("Text");
static const FString LEAF_SPEAKER_KEY("Speaker");
static const FString LEAF_SOUND_KEY("Sound");
static const FString LEAF_EMOTION_KEY("Emotion");
static const FString LEAF_ACTIONS_KEY("Actions");

typedef TSharedRef<FJsonObject> JsonObjectRef;
typedef TSharedPtr<FJsonObject> JsonObjectPtr;

typedef TArray<FString> Actions_t;
// Last bunch of data and instructions in a script
struct ScriptLeaf_s
{
    FString Text;
    FString Speaker;
    FString Sound;
    FString Emotion;
    Actions_t Actions;
};
typedef TSharedRef<ScriptLeaf_s> ScriptLeaf_t;

typedef TArray<ScriptLeaf_t> Dialog_t;
typedef TArray<ScriptLeaf_t> Answers_t;

struct Condition_s
{
    Dialog_t dialog;
    Answers_t answers;
    Actions_t actions;
};
typedef TSharedRef<Condition_s> Condition_t;

typedef TSharedPtr<TMap<FString, Condition_t>> CardObject_t;
typedef TSharedPtr<TMap<FString, CardObject_t>> Act_t;
typedef TSharedPtr<TMap<FString, Act_t>> Script_t;


Actions_t load_actions(JsonObjectRef json_leaf)
{
    const TArray<TSharedPtr<FJsonValue>>* json_values_array;
    Actions_t actions;
    if (json_leaf->TryGetArrayField(ACTIONS_KEY, json_values_array))
    {
        check(json_values_array);
        for (auto json_value : *json_values_array)
            actions.Add(json_value->AsString());
    }
    return actions;
}

ScriptLeaf_t load_leaf(JsonObjectRef json_leaf)
{
    ULOG(Log, "Restoring a Script leaf");

    ScriptLeaf_t leaf;
    try
    {
        auto json_leaf_dict = json_leaf->Values;

        json_leaf->TryGetStringField(LEAF_TEXT_KEY, leaf->Text);
        json_leaf->TryGetStringField(LEAF_SPEAKER_KEY, leaf->Speaker);
        json_leaf->TryGetStringField(LEAF_SOUND_KEY, leaf->Sound);
        json_leaf->TryGetStringField(LEAF_EMOTION_KEY, leaf->Emotion);


        //if ()
        //{
        //    check(json_values_array);
        //    for (auto json_value : *json_values_array)
        //        dialog.Add(load_leaf(json_value->AsObject().ToSharedRef()));
        //}
    }
    catch (const std::exception& ex)
        {
            ULOG(Error, "Error accured while loading Leaf:\n%s", ex.what());
        }
    
    ULOG(Log, "Restoring a Script leaf finished");
    return leaf;
}

Condition_t load_condition(JsonObjectRef json_conditions)
{
    ULOG(Log, "Restoring a Condition");

    Condition_t condition;

    for (auto json_condition : json_conditions->Values)
    {
        ULOG(Log, "Key found: %s", *json_condition.Key);

        try
        {
            auto& json_condition_val = json_condition.Value->AsObject();
            const TArray<TSharedPtr<FJsonValue>>* json_values_array;

            if (json_condition_val->TryGetArrayField(DIALOG_KEY, json_values_array))
            {
                check(json_values_array);
                for (auto json_value : *json_values_array)
                    condition->dialog.Add(load_leaf(json_value->AsObject().ToSharedRef()));
            }

            if (json_condition_val->TryGetArrayField(ANSWERS_KEY, json_values_array))
            {
                check(json_values_array);
                for (auto json_value : *json_values_array)
                    condition->answers.Add(load_leaf(json_value->AsObject().ToSharedRef()));
            }

            if (json_condition_val->TryGetArrayField(ACTIONS_KEY, json_values_array))
            {
                check(json_values_array);
                for (auto json_value : *json_values_array)
                    condition->actions.Add(json_value->AsString());
            }
        }
        catch (const std::exception& ex)
        {
            ULOG(Error, "Error accured while loading Condition:\n%s", ex.what());
        }
    }

    ULOG(Log, "Restoring a Condition finished");
    return condition;
}

CardObject_t load_card_object(JsonObjectRef json_card_object)
{
    ULOG(Log, "Restoring a Card object");

    CardObject_t card_objects;

    for (auto json_condition_val : json_card_object->Values)
    {
        ULOG(Log, "Key found: %s", *json_condition_val.Key);
        try
        {
            auto condition_object = load_condition(json_condition_val.Value->AsObject().ToSharedRef());
            card_objects->Add(json_condition_val.Key, condition_object);
        }
        catch (const std::exception& ex)
        {
            ULOG(Error, "Error accured while loading Card object:\n%s", ex.what());
        }
    }

    ULOG(Log, "Restoring a Card object finished");
    return card_objects;
}

Act_t load_act(JsonObjectRef json_act)
{
    ULOG(Log, "Restoring an Act");

    Act_t card_objects;

    for (auto json_card_val : json_act->Values)
    {
        ULOG(Log, "Key found: %s", *json_card_val.Key);
        try
        {
            auto act = load_card_object(json_card_val.Value->AsObject().ToSharedRef());
            card_objects->Add(json_card_val.Key, act);
        }
        catch (const std::exception& ex)
        {
            ULOG(Error, "Error while loading Act: \n%s", ex.what());
        }
    }

    ULOG(Log, "Restoring an Act finished");
    return card_objects;
}

Script_t load_script(JsonObjectRef json_root)
{
    ULOG(Log, "Loading a Script");
    //FString ExampleString = json_root->GetStringField("exampleString");
    //auto json_parsed_ptr = json_root.Get();

    //TArray<FString> keys;
    //json_root->Values.GetKeys(keys);
    //for (auto& key : keys)
    //{
    //    ULOG(Warning, "Key: %s", *key);
    //}

    Script_t script;

    for (auto json_act_val : json_root->Values)
    {
        ULOG(Log, "Key found: %s", *json_act_val.Key);
        try
        {
            auto act = load_act(json_act_val.Value->AsObject().ToSharedRef());
            script->Add(json_act_val.Key, act);
        }
        catch (const std::exception& ex)
        {
            ULOG(Error, "Error while loading Script: \n%s", ex.what());
        }
    }
    
    ULOG(Log, "Loading a Script finished");
    return script;
}

void ParseExample()
{
    ULOG(Log, "Enter the ParseExample()");

    const FString SCRIPT_FILENAME("Data/script.json");
    const FString script_full_path = FPaths::ProjectDir().Append(SCRIPT_FILENAME);

    FString json_raw;
    if (FFileHelper::LoadFileToString(json_raw, *script_full_path) == false)
    {
        ULOG(Error, "Failed LoadJson : %s", *script_full_path);
    }

    JsonObjectPtr json_object;
    TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(json_raw);

    if (FJsonSerializer::Deserialize(json_reader, json_object) && json_object.IsValid())
    {
        load_script(json_object.ToSharedRef());
    }
    else
    {
        ULOG(Error, "Unable to load script from file \"%s\"", *script_full_path);
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

void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard* interact_card, CardType& card_type, TArray<FDialogUnit>& dialog, TArray<FText>& Answers)
{
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, TArray<FDialogUnit>& dialog, ActionWithCard& action_with_card)
{
}

