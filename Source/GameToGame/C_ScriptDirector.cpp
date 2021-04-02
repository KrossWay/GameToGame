#include "C_ScriptDirector.h"
#include <Templates/SharedPointer.h>
#include <Json.h>

const FString SCRIPT_FILENAME("script.json");

AC_ScriptDirector::AC_ScriptDirector()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    script = load_script_from_file(SCRIPT_FILENAME);
}

void AC_ScriptDirector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AC_ScriptDirector::BeginPlay()
{
    // Called when the game starts or when spawned
    Super::BeginPlay();

}

void AC_ScriptDirector::read_story_from_file(const FString story_filename)
{

}

void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard* interact_card, CardType& card_type, TArray<FDialogUnit>& dialog, TArray<FText>& Answers)
{
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, TArray<FDialogUnit>& dialog, ActionWithCard& action_with_card)
{
}

