#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "common_structures.h"
#include "C_Script.h"
#include "C_ScriptDirector.generated.h"


UCLASS()
class GAMETOGAME_API AC_ScriptDirector : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AC_ScriptDirector();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SwitchAct(const FString &act_name);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ProcessDialog(const AC_MasterCard *interact_card, CardType& card_type, TArray<FDialogUnit>& dialog, TArray<FButtonText>& answers);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ProcessDialogResult(const int32 answer_idx, ActionWithCard &action_with_card, FString &new_card_name);

private:
    Script_p script;
    FString current_act;
    TArray<FString> notes;

    Actions_p stored_condition_actions;
    TArray<Actions_p> stored_answers_actions;

    void fill_dialog_output(const Dialog_t& dialog_source, TArray<FDialogUnit>& dialog);
    void process_output_answers(const Answers_t& answers_source, TArray<FButtonText>& answers);
    bool is_conditions_proper(const Conditions_t& conditions);
    void process_actions(const Actions_p& actions, ActionWithCard *card_action = nullptr, FString *card_to_replace = nullptr);
};
