#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"



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
typedef TArray<FString> Conditions_t;
typedef TArray<ScriptLeaf_p> Dialog_t;
typedef TArray<ScriptLeaf_p> Answers_t;

struct ConditionItem_s
{
    Conditions_t conditions;
    Dialog_t dialog;
    Answers_t answers;
    Actions_p actions;
};

typedef TSharedPtr<ConditionItem_s> ConditionItem_p;

typedef TArray<ConditionItem_p> CardObject_t;
typedef TSharedPtr<CardObject_t> CardObject_p;
typedef TMap<FString, CardObject_p> Act_t;
typedef TSharedPtr<Act_t> Act_p;

typedef TMap<FString, Act_p> Script_t;
typedef TSharedPtr<Script_t> Script_p;

Script_p load_script_from_file(const FString script_filename);
