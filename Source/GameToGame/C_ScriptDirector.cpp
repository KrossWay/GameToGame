#include "C_ScriptDirector.h"
#include <Templates/SharedPointer.h>
#include "common.h"
#include <Json.h>

const FString SCRIPT_FILENAME("script.json");

AC_ScriptDirector::AC_ScriptDirector()
{
    PrimaryActorTick.bCanEverTick = true;

    notes = { "Default" };
    current_act = "Act1";
    script = load_script_from_file(SCRIPT_FILENAME);
}

void AC_ScriptDirector::SwitchAct_Implementation(const FString &act_name)
{
    current_act = act_name;
    // Maybe return some settings for the act
}

bool is_condition_proper(ConditionItem_p conditions)
{

    return true;
}

void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard* interact_card, CardType& card_type, TArray<FDialogUnit>& dialog, TArray<FText>& answers)
{
    const FString& card_name = interact_card->card_name;

    const auto& act = script->FindChecked(current_act);
    const auto& card = act->FindChecked(card_name);

    // TODO: Make conditions a list

    for (auto& condition_key : *card)
    {
        //const auto cond_idx = notes.FindLast(condition_key);
        //if (cond_idx != INDEX_NONE)
        //{
        //    // Appropriate condition has been found
        //    const auto& condition = card->FindChecked(condition_key);
        //    check(condition.IsValid());

        //    fill_dialog_output(condition->dialog, dialog);
        //    process_answers(condition->answers, answers);
        //}
        //else
        //    ULOG(Error, "Undex for the condition \"%s\" not found.", *condition_key);
    }
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, ActionWithCard& action_with_card)
{
}

void AC_ScriptDirector::fill_dialog_output(const Dialog_t& dialog_source, TArray<FDialogUnit>& dialog)
{
    for (const auto& dialog_leaf : dialog_source)
    {
        SpeakerType speaker_type = SpeakerType::STORYTELLER;
        FText text_to_print = FText::FromString("I'm wrong string. Seems like you've found the crown.");
        FString sound = "";
        CardEmotion emotion = CardEmotion::EMOTION_DEFAULT;

        if ("Character" == dialog_leaf->speaker)
            speaker_type = SpeakerType::INTERLOCUTOR;
        else if ("Hero" == dialog_leaf->speaker)
            speaker_type = SpeakerType::HERO;

        text_to_print = FText::FromString(dialog_leaf->text);
        sound = dialog_leaf->sound;

        static const TMap<FString, CardEmotion> emotions_map = {
            {"Happy", CardEmotion::EMOTION_HAPPY},
            {"Angry", CardEmotion::EMOTION_ANGRY},
            {"Wondering", CardEmotion::EMOTION_WONDERING},
            {"None", CardEmotion::EMOTION_NONE}
        };
        const auto* found_emotion = emotions_map.Find(dialog_leaf->emotion);
        if (found_emotion)
            emotion = *found_emotion;

        dialog.Add(FDialogUnit(speaker_type, text_to_print, sound, emotion));
    }
}

void AC_ScriptDirector::process_answers(const Answers_t& answers_source, TArray<FText>& answers)
{
    for (const auto& answer_leaf : answers_source)
    {
        FText text_to_print = FText::FromString("I'm wrong string. Seems like you've found the crown.");
        text_to_print = FText::FromString(answer_leaf->text);

        // TODO: store answer actions

        // TODO: add answers conditionally
        answers.Add(text_to_print);
    }
}

