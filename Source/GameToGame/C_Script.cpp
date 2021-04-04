#include "C_Script.h"
#include "common.h"


static const FString CONDITION_KEY("Conditions");
static const FString DIALOG_KEY("Dialog");
static const FString ANSWERS_KEY("Answers");
static const FString ACTIONS_KEY("Actions");

static const FString LEAF_TEXT_KEY("Text");
static const FString LEAF_FULL_TEXT_KEY("FullText");
static const FString LEAF_SPEAKER_KEY("Speaker");
static const FString LEAF_SOUND_KEY("Sound");
static const FString LEAF_EMOTION_KEY("Emotion");

typedef TSharedRef<FJsonObject> JsonObjectRef;
typedef TSharedPtr<FJsonObject> JsonObjectPtr;
typedef TSharedRef<FJsonValue> JsonValueRef;
typedef TSharedPtr<FJsonValue> JsonValuePtr;

Actions_p load_actions(JsonObjectRef json_leaf)
{
    ULOG(Log, "Loading Actions.");
    const TArray<TSharedPtr<FJsonValue>>* json_values_array;
    Actions_p actions = MakeShared<Actions_t>();
    if (json_leaf->TryGetArrayField(ACTIONS_KEY, json_values_array))
    {
        check(json_values_array);
        for (auto json_value : *json_values_array)
            actions->Add(json_value->AsString());
    }
    return actions;
}

ScriptLeaf_p load_leaf(JsonObjectRef json_leaf)
{
    ULOG(Log, "Loading a Script leaf");

    ScriptLeaf_p leaf = MakeShared<ScriptLeaf_s>();
    auto json_leaf_dict = json_leaf->Values;

    json_leaf->TryGetStringField(LEAF_TEXT_KEY, leaf->text);
    json_leaf->TryGetStringField(LEAF_FULL_TEXT_KEY, leaf->full_text);
    if (leaf->full_text.IsEmpty())
        leaf->full_text = leaf->text;
    json_leaf->TryGetStringField(LEAF_SPEAKER_KEY, leaf->speaker);
    json_leaf->TryGetStringField(LEAF_SOUND_KEY, leaf->sound);
    json_leaf->TryGetStringField(LEAF_EMOTION_KEY, leaf->emotion);
    leaf->actions = load_actions(json_leaf);
    
    ULOG(Log, "Loading a Script leaf finished");
    return leaf;
}

ConditionItem_p load_condition_item(JsonObjectRef json_conditions)
{
    ULOG(Log, "Loading a Condition");

    ConditionItem_p condition = MakeShared<ConditionItem_s>();
    const TArray<TSharedPtr<FJsonValue>>* json_values_array;

    if (json_conditions->TryGetArrayField(CONDITION_KEY, json_values_array))
    {
        ULOG(Log, "Loading Conditions.");
        check(json_values_array);
        for (auto json_value : *json_values_array)
        {
            auto tmp_json_obj = json_value->AsString();
            if (!tmp_json_obj.IsEmpty())
                condition->conditions.Add(tmp_json_obj);
            else
                ULOG(Error, "Error while loading Conditions.");
        }
    }
    else
    {
        ULOG(Error, "Unable to find Conditions (required) field in the given condition item.");
    }

    if (json_conditions->TryGetArrayField(DIALOG_KEY, json_values_array))
    {
        ULOG(Log, "Loading Dialog.");
        check(json_values_array);
        for (auto json_value : *json_values_array)
        {
            auto tmp_json_obj = json_value->AsObject();
            if (tmp_json_obj)
                condition->dialog.Add(load_leaf(tmp_json_obj.ToSharedRef()));
            else
                ULOG(Error, "Error while loading Dialog.");
        }
    }
    else
    {
        ULOG(Error, "Unable to find Dialog (required) field in the given condition item.");
    }

    if (json_conditions->TryGetArrayField(ANSWERS_KEY, json_values_array))
    {
        ULOG(Log, "Loading Answers.");
        check(json_values_array);
        for (auto json_value : *json_values_array)
        {
            auto tmp_json_obj = json_value->AsObject();
            if (tmp_json_obj)
                condition->answers.Add(load_leaf(tmp_json_obj.ToSharedRef()));
            else
                ULOG(Error, "Error while loading Answers.");
        }
    }

    condition->actions = load_actions(json_conditions);

    return condition;
}

CardObject_p load_card_object(TArray<TSharedPtr<FJsonValue>> &json_card_object)
{
    ULOG(Log, "Loading a Card object");

    CardObject_p card_object = MakeShared<CardObject_t>();

    for (auto json_value : json_card_object)
    {
        auto tmp_json_obj = json_value->AsObject();
        if (tmp_json_obj)
            card_object->Add(load_condition_item(tmp_json_obj.ToSharedRef()));
        else
            ULOG(Error, "Error while loading Dialog.");
    }

    ULOG(Log, "Loading a Card object finished");
    return card_object;
}

Act_p load_act(JsonObjectRef json_act)
{
    ULOG(Log, "Loading an Act");

    Act_p card_objects = MakeShared<Act_t>();

    for (auto json_card_val : json_act->Values)
    {
        ULOG(Log, "Key found: %s", *json_card_val.Key);
        auto tmp_json_obj = json_card_val.Value->AsArray();
        if (tmp_json_obj.Num() > 0)
        {
            auto card_object = load_card_object(tmp_json_obj);
            card_objects->Add(json_card_val.Key, card_object);
        }
        else
            ULOG(Error, "Error while loading Act.");
    }

    ULOG(Log, "Loading an Act finished");
    return card_objects;
}

Script_p load_script(JsonObjectRef json_root)
{
    ULOG(Log, "Loading a Script");

    Script_p script = MakeShared<Script_t>();

    for (auto json_act_val : json_root->Values)
    {
        ULOG(Log, "Key found: %s", *json_act_val.Key);

        auto tmp_json_obj = json_act_val.Value->AsObject();
        if (tmp_json_obj)
        {
            auto act = load_act(tmp_json_obj.ToSharedRef());
            script->Add(json_act_val.Key, act);
        }
        else
            ULOG(Error, "Error while loading Script.");
    }
    
    ULOG(Log, "Loading a Script finished");
    return script;
}

Script_p load_script_from_file(const FString script_filename)
{
    ULOG(Log, "Script parsing has begun.");

    
    const FString script_full_path = FPaths::ProjectContentDir().Append(FPaths::Combine(DATA_FOLDER_NAME, script_filename));
    Script_p result_script = MakeShared<Script_t>();

    FString json_raw;
    if (FFileHelper::LoadFileToString(json_raw, *script_full_path) == false)
    {
        ULOG(Error, "Failed LoadJson : %s", *script_full_path);
    }

    JsonObjectPtr json_object;
    TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(json_raw);
    if (FJsonSerializer::Deserialize(json_reader, json_object) && json_object.IsValid())
    {
        auto loaded_script = load_script(json_object.ToSharedRef());
        if (loaded_script.IsValid())
            result_script->Append(*loaded_script);
    }
    else
    {
        ULOG(Error, "Unable to load script from file \"%s\"\nJson error messsage:\"%s\"", *script_full_path, *json_reader->GetErrorMessage());
    }

    ULOG(Log, "Script parsing has begun.");
    return result_script;
}
