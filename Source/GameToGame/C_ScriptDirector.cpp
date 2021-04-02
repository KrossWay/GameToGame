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

typedef TSharedRef<FJsonObject> JsonObjectRef;
typedef TSharedPtr<FJsonObject> JsonObjectPtr;
typedef TSharedRef<FJsonValue> JsonValueRef;
typedef TSharedPtr<FJsonValue> JsonValuePtr;

typedef TArray<FString> Actions_t;
typedef TSharedPtr<Actions_t> Actions_p;
// Last bunch of data and instructions in a script
struct ScriptLeaf_s
{
    FString text;
    FString speaker;
    FString sound;
    FString emotion;
    Actions_p actions;
};
typedef TSharedPtr<ScriptLeaf_s> ScriptLeaf_p;

typedef TArray<ScriptLeaf_p> Dialog_t;
typedef TArray<ScriptLeaf_p> Answers_t;

struct Condition_s
{
    Dialog_t dialog;
    Answers_t answers;
    Actions_p actions;
};
typedef TSharedPtr<Condition_s> Condition_p;

typedef TMap<FString, Condition_p> CardObject_t;
typedef TSharedPtr<CardObject_t> CardObject_p;
typedef TMap<FString, CardObject_p> Act_t;
typedef TSharedPtr<Act_t> Act_p;
typedef TMap<FString, Act_p> Script_t;
typedef TSharedPtr<Script_t> Script_p;


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
    json_leaf->TryGetStringField(LEAF_SPEAKER_KEY, leaf->speaker);
    json_leaf->TryGetStringField(LEAF_SOUND_KEY, leaf->sound);
    json_leaf->TryGetStringField(LEAF_EMOTION_KEY, leaf->emotion);
    leaf->actions = load_actions(json_leaf);
    
    ULOG(Log, "Loading a Script leaf finished");
    return leaf;
}

Condition_p load_condition(JsonObjectRef json_conditions)
{
    ULOG(Log, "Loading a Condition");

    Condition_p condition = MakeShared<Condition_s>();
    const TArray<TSharedPtr<FJsonValue>>* json_values_array;

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

CardObject_p load_card_object(JsonObjectRef json_card_object)
{
    ULOG(Log, "Loading a Card object");

    CardObject_p card_objects = MakeShared<CardObject_t>();

    for (auto json_condition_val : json_card_object->Values)
    {
        ULOG(Log, "Key found: %s", *json_condition_val.Key);
        auto tmp_json_obj = json_condition_val.Value->AsObject();
        if (tmp_json_obj)
        {
            auto act = load_condition(tmp_json_obj.ToSharedRef());
            card_objects->Add(json_condition_val.Key, act);
        }
        else
            ULOG(Error, "Error while loading Card object.");
    }

    ULOG(Log, "Loading a Card object finished");
    return card_objects;
}

Act_p load_act(JsonObjectRef json_act)
{
    ULOG(Log, "Loading an Act");

    Act_p card_objects = MakeShared<Act_t>();

    for (auto json_card_val : json_act->Values)
    {
        ULOG(Log, "Key found: %s", *json_card_val.Key);
        auto tmp_json_obj = json_card_val.Value->AsObject();
        if (tmp_json_obj)
        {
            auto act = load_card_object(tmp_json_obj.ToSharedRef());
            card_objects->Add(json_card_val.Key, act);
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

Script_p ParseExample()
{
    ULOG(Log, "Enter the ParseExample()");

    const FString SCRIPT_FILENAME("Data/script.json");
    const FString script_full_path = FPaths::ProjectContentDir().Append(SCRIPT_FILENAME);

    FString json_raw;
    if (FFileHelper::LoadFileToString(json_raw, *script_full_path) == false)
    {
        ULOG(Error, "Failed LoadJson : %s", *script_full_path);
    }

    JsonObjectPtr json_object;
    TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(json_raw);

    if (FJsonSerializer::Deserialize(json_reader, json_object) && json_object.IsValid())
    {
        return load_script(json_object.ToSharedRef());
    }
    else
    {
        ULOG(Error, "Unable to load script from file \"%s\"\nJson error messsage:\"%s\"", *script_full_path, *json_reader->GetErrorMessage());
    }
    return Script_p(nullptr);
}

AC_ScriptDirector::AC_ScriptDirector()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    auto script = ParseExample();
    auto act1 = script.Get()['Act1'];
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

