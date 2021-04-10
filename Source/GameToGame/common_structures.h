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

UENUM(BlueprintType)
enum class TestMode : uint8
{
    TEST_MODE_SELECT_CARD = 0 UMETA(DisplayName = "Select a card"),
    TEST_MODE_SELECT_ACT = 1 UMETA(DisplayName = "Select an act"),
};

USTRUCT(Blueprintable)
struct FButtonText 
{
    GENERATED_USTRUCT_BODY()
    
    FButtonText() {}
    FButtonText(FText text, FText full_text)
        : text(text)
        , full_text(full_text)
    {}

    UPROPERTY(BlueprintReadWrite)
    FText text;
    UPROPERTY(BlueprintReadWrite)
    FText full_text;

    FString to_string() const
    {
        FString result("Button text:\n");
        result.Append("Short: " + text.ToString() + "\n");
        result.Append("Full: " + full_text.ToString() + "\n");
        return result;
    }
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

    FString to_string() const
    {
        FString result("Dialog unit:\n");
        result.Append("Speaker type: " + FString::FromInt((int)speaker_type) + "\n");
        result.Append("Text to print: " + text_to_print.ToString() + "\n");
        //result.Append("Emotion: " + FString::FromInt((int)emotion) + "\n");
        return result;
    }
};

