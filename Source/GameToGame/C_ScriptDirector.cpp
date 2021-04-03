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

void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard* interact_card, CardType& card_type, TArray<FDialogUnit>& dialog, TArray<FText>& answers)
{
    ULOG(Log, "Process dialog started.");

    const FString& card_name = interact_card->card_name;
    const auto& act = script->FindChecked(current_act);
    const auto& card = act->FindChecked(card_name);

    ConditionItem_p item_to_apply(nullptr);
    for (const auto& condition_item : *card)
    {
        if (is_condition_proper(condition_item->conditions))
        {
            item_to_apply = condition_item;
        }
    }
    
    if (!item_to_apply.IsValid())
    {
        ULOG(Error, "Unable to find proper condition for given card: \"%s\".", *card_name);
        ULOG(Error, "Condition list:");
        for (const auto& note : notes)
            ULOG(Error, "%s", *note);
        return;
    }

    fill_dialog_output(item_to_apply->dialog, dialog);
    process_output_answers(item_to_apply->answers, answers);

    ULOG(Log, "Process dialog finished.");
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, ActionWithCard& action_with_card)
{
    ULOG(Log, "Process dialog result started.");

    if (stored_answers_actions.Num())
    {
        process_actions(stored_answers_actions[answer_idx]);
    }
    else
        ULOG(Log, "Nothing to process.");

    ULOG(Log, "Process dialog result finished.");
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

void AC_ScriptDirector::process_output_answers(const Answers_t& answers_source, TArray<FText>& answers)
{
    stored_answers_actions.Empty();
    for (const auto& answer_leaf : answers_source)
    {
        FText text_to_print = FText::FromString("I'm wrong string. Seems like you've found the crown.");
        text_to_print = FText::FromString(answer_leaf->text);
        stored_answers_actions.Add(answer_leaf->actions);

        // TODO: add answers conditionally
        answers.Add(text_to_print);
    }
}

bool AC_ScriptDirector::is_condition_proper(const Conditions_t &conditions)
{
    if (conditions.Contains("Default"))
        return true;

    for (const auto& cond : conditions)
    {
        if (notes.Contains(cond))
            return true;
    }

    return false;
}

void AC_ScriptDirector::process_actions(const Actions_p& Actions)
{
    throw std::logic_error("The method or operation is not implemented.");
}

