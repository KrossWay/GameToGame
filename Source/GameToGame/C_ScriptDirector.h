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
    void ProcessDialog(const AC_MasterCard *interact_card, CardType &card_type, TArray<FDialogUnit> &dialog, TArray<FText> &Answers);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ProcessDialogResult(const int32 answer_idx, TArray<FDialogUnit> &dialog, ActionWithCard &action_with_card);

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    void read_story_from_file(const FString story_filename);

    Script_p script;
};
