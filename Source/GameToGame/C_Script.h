#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"


struct Condition_s;
typedef TSharedPtr<Condition_s> Condition_p;

typedef TMap<FString, Condition_p> CardObject_t;
typedef TSharedPtr<CardObject_t> CardObject_p;
typedef TMap<FString, CardObject_p> Act_t;
typedef TSharedPtr<Act_t> Act_p;

typedef TMap<FString, Act_p> Script_t;
typedef TSharedPtr<Script_t> Script_p;

Script_p load_script_from_file(const FString script_filename);
