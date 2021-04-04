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
    ULOG(Log, "Switching act to \"%s\".", *act_name);
    current_act = act_name;
    // Maybe return some settings for the act
}

void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard* interact_card, CardType& card_type, TArray<FDialogUnit>& dialog, TArray<FButtonText>& answers)
{
    ULOG(Log, "Process dialog started.");

    if (!interact_card)
    {
        ULOG(Error, "OMG. No card has been passed. Attention!");
        return;
    }

    const FString& card_name = interact_card->card_name;
    const auto& act = script->FindChecked(current_act);
    const auto* card_found = act->Find(card_name);
    if (card_found)
    {
        ULOG(Log, "Processing card with card name: \"%s\"", *card_name);
    }
    else
    {
        ULOG(Error, "Get out with this unknown card name! \"%s\"", *card_name);
        return;
    }
    const auto& card_obj = *card_found;

    ConditionItem_p item_to_apply(nullptr);
    // TODO: reverse for loop
    for (const auto& condition_item : *card_obj)
    {
        if (is_conditions_proper(condition_item->conditions))
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
    stored_condition_actions = MakeShared<Actions_t>(*item_to_apply->actions);

    for (const auto& du : dialog)
        ULOG(Log, "%s", *du.to_string());
    for (const auto& ans : answers)
        ULOG(Log, "%s", *ans.to_string());


    ULOG(Log, "Process dialog finished.");
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, ActionWithCard& action_with_card, FString &new_card_name)
{
    ULOG(Log, "Process dialog result started.");
    ULOG(Log, "Answer index: %d.", answer_idx);

    if (stored_answers_actions.Num())
    {
        if(stored_condition_actions)
            stored_condition_actions->Insert(*stored_answers_actions[answer_idx], 0);
        else
            ULOG(Error, "Are you insane? Call the ProcessDialog() first.");
    }
    else
        ULOG(Log, "Nothing to process on the answer.");

    process_actions(stored_condition_actions, &action_with_card, &new_card_name);

    ULOG(Log, "Process dialog result finished.");
}

void AC_ScriptDirector::fill_dialog_output(const Dialog_t& dialog_source, TArray<FDialogUnit>& dialog)
{
    ULOG_FSTART;

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

    ULOG_FFINISH;
}

void AC_ScriptDirector::process_output_answers(const Answers_t& answers_source, TArray<FButtonText>& answers)
{
    ULOG_FSTART;

    stored_answers_actions.Empty();
    for (const auto& answer_leaf : answers_source)
    {
        FText text_to_show = FText::FromString("I'm wrong string. Seems like you've found the crown.");
        FText text_to_print = FText::FromString("I'm wrong string. Seems like you've found the crown.");
        text_to_show = FText::FromString(answer_leaf->text);
        text_to_print = FText::FromString(answer_leaf->full_text);
        stored_answers_actions.Add(answer_leaf->actions);

        // TODO: add answers conditionally
        answers.Add(FButtonText(text_to_show, text_to_print));
    }

    ULOG_FFINISH;
}

bool AC_ScriptDirector::is_conditions_proper(const Conditions_t &conditions)
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

void AC_ScriptDirector::process_actions(const Actions_p& actions, ActionWithCard *card_action, FString *new_card_name)
{
    ULOG_FSTART;
    ULOG(Log, "Actions to process:\n");
    for (const auto& ac : *actions)
    {
        ULOG(Log, "%s\n", *ac);
    }

    static const FString ACTION_NOTE("Note");
    static const FString ACTION_IF("If");
    static const FString ACTION_SPEAK_AGAIN("SpeakAgain");
    static const FString ACTION_NEXT_ACT("NextAct");
    static const FString ACTION_SWITCH_CARD("SwitchCard");

    if (card_action)
        *card_action = ActionWithCard::CARD_ACTION_END_DIALOG;
    if (new_card_name)
        *new_card_name = "";

    for (const auto& action : *actions)
    {
        if (action.StartsWith(ACTION_NOTE))
        {
            FString note;
            action.Split(" ", nullptr, &note);
            notes.Add(note);
            ULOG(Log, "Storing new note: \"%s\"", *note);
        }
        else if (action.StartsWith(ACTION_IF))
        {
            FString statement;
            action.Split(" ", nullptr, &statement);
            FString condition, note;
            statement.Split(" ", &condition, &note);
            ULOG(Log, "Statement processing: \"%s\"", *action);

            if (notes.Contains(condition))
            {
                notes.Add(note);
                ULOG(Log, "Condition is satisfied. Storing new note: \"%s\"", *note);
            }
            else
                ULOG(Log, "Condition is not satisfied: \"%s\"", *condition);
        }
        else if (action.StartsWith(ACTION_SPEAK_AGAIN))
        {
            if (card_action)
                *card_action = ActionWithCard::CARD_ACTION_CONTINUE_DIALOG;
            ULOG(Log, "Speak again action with card: \"%s\"", *action);
        }
        else if (action.StartsWith(ACTION_NEXT_ACT))
        {
            if (card_action)
                *card_action = ActionWithCard::CARD_ACTION_NEXT_ACT;
            ULOG(Log, "Next act moving proposing");
        }
        else if (action.StartsWith(ACTION_SWITCH_CARD))
        {
            FString new_card_name_statement;
            action.Split(" ", nullptr, &new_card_name_statement);
            if (new_card_name)
                *new_card_name = new_card_name_statement;
            ULOG(Log, "Propose to swap card with new one: \"%s\"", **new_card_name);
        }
        else
            ULOG(Error, "Unknown action: \"%s\"", *action);
    }

    ULOG_FFINISH;
}

