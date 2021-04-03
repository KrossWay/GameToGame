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
    STORYTELLER = 0 UMETA(DisplayName = "Рассказчик"),
    HERO = 1 UMETA(DisplayName = "Герой"),
    INTERLOCUTOR = 2 UMETA(DisplayName = "Собеседник")
};

UENUM(BlueprintType)
enum class ActionWithCard : uint8
{
    CARD_ACTION_END_DIALOG = 0 UMETA(DisplayName = "Закончить диалог"),
    CARD_ACTION_CONTINUE_DIALOG = 1 UMETA(DisplayName = "Продолжить диалог"),
    CARD_ACTION_NEXT_ACT = 2 UMETA(DisplayName = "Перейти в следующий акт"),
};

UENUM(BlueprintType)
enum class CardEmotion : uint8
{
    EMOTION_DEFAULT = 0 UMETA(DisplayName = "Простая (Default)"),
    EMOTION_HAPPY = 1 UMETA(DisplayName = "Радость"),
    EMOTION_ANGRY = 2 UMETA(DisplayName = "Злость"),
    EMOTION_WONDERING = 3 UMETA(DisplayName = "Удивление"),
    EMOTION_NONE = 255 UMETA(DisplayName = "Пустота")
};

USTRUCT(Blueprintable)
struct FDialogUnit
{
    GENERATED_USTRUCT_BODY()

    FDialogUnit()
    : speaker_type(SpeakerType::STORYTELLER)
    , text_to_print()
    , sound("")
    , emotion(CardEmotion::EMOTION_DEFAULT)
    {}

    FDialogUnit(SpeakerType speaker_type, FText text_to_print, FString sound, CardEmotion emotion)
    : speaker_type(speaker_type)
    , text_to_print(text_to_print)
    , sound(sound)
    , emotion(emotion)
    {}

    UPROPERTY(BlueprintReadWrite)
    SpeakerType speaker_type;

    UPROPERTY(BlueprintReadWrite)
    FText text_to_print;

    UPROPERTY(BlueprintReadWrite)
    FString sound;

    UPROPERTY(BlueprintReadWrite)
    CardEmotion emotion;
};

