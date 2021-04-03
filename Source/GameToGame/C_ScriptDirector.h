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
    void ProcessDialog(const AC_MasterCard *interact_card, CardType &card_type, TArray<FDialogUnit> &dialog, TArray<FText> &answers);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ProcessDialogResult(const int32 answer_idx, ActionWithCard &action_with_card);

private:
    Script_p script;
    FString current_act;
    TArray<FString> notes;

    void fill_dialog_output(const Dialog_t& dialog_source, TArray<FDialogUnit>& dialog);
    void process_answers(const Answers_t& answers_source, TArray<FText>& answers);
};
