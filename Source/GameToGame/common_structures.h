#pragma once

#include "Sound/SoundCue.h"
#include "C_MasterCard.h"
#include "common_structures.generated.h"


UENUM(BlueprintType)
enum class CardType : uint8
{
    CHARACTER_CARD = 0 UMETA(DisplayName = "Персонаж"),
    ITEM_CARD = 1 UMETA(DisplayName = "Предмет"),
    EVENT_CARD = 2 UMETA(DisplayName = "Событие")
};


UENUM(BlueprintType)
enum class SpeakerType : uint8
{
    INTERLOCUTOR = 0 UMETA(DisplayName = "Собеседник"),
    HERO = 1 UMETA(DisplayName = "Герой"),
    STORYTELLER = 2 UMETA(DisplayName = "Рассказчик")
};


UENUM(BlueprintType)
enum class ActionWithCard : uint8
{
    TAKE_CARD = 0 UMETA(DisplayName = "Взять карту"),
    KILL_CARD = 1 UMETA(DisplayName = "Разрушить карту"),
    BACK_CARD = 2 UMETA(DisplayName = "Вернуть карту на стол")
};

USTRUCT(Blueprintable)
struct FDialogUnit
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    SpeakerType speaker_type;

    UPROPERTY(BlueprintReadWrite)
    FText text_to_print; // TODO: read this https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/FText/index.html

    UPROPERTY(BlueprintReadWrite)
    USoundCue *sound;
};

