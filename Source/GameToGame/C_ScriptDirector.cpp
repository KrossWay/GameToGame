// Fill out your copyright notice in the Description page of Project Settings.


#include "C_ScriptDirector.h"

// Sets default values
AC_ScriptDirector::AC_ScriptDirector()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AC_ScriptDirector::BeginPlay()
{
    Super::BeginPlay();
    
}

// Called every frame
void AC_ScriptDirector::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}


void AC_ScriptDirector::ProcessDialog_Implementation(const AC_MasterCard *interact_card, CardType &card_type, TArray<FDialogUnit> &dialog, bool &has_weapon, TArray<FText> &Answers)
{
}

void AC_ScriptDirector::ProcessDialogResult_Implementation(const int32 answer_idx, TArray<FDialogUnit> &dialog, ActionWithCard &action_with_card)
{
}

