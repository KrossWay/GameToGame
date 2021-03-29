// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "C_MasterCard.h"
#include "C_ScriptDirector.generated.h"


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

UCLASS()
class GAMETOGAME_API AC_ScriptDirector : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AC_ScriptDirector();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ProcessDialog(const AC_MasterCard *interact_card, TArray<FDialogUnit> &dialog, bool &has_weapon, TArray<FText> &Answers);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void ProcessDialogResult(const int32 answer_idx, TArray<FDialogUnit> &dialog, ActionWithCard &action_with_card);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
