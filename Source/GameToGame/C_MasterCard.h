// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "C_MasterCard.generated.h"

UENUM(BlueprintType)
enum class SpeakerType : uint8
{
    INTERLOCUTOR = 0 UMETA(DisplayName = "Собеседник"),
    HERO = 1 UMETA(DisplayName = "Герой"),
    STORYTELLER = 2 UMETA(DisplayName = "Рассказчик")
};

UCLASS(Blueprintable)
class UDialogUnit : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    SpeakerType speaker_type;

    UPROPERTY(EditAnywhere)
    FText text_to_print; // TODO: read this https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/StringHandling/FText/index.html

    UPROPERTY(EditAnywhere)
    USoundCue *sound;
};

UCLASS()
class GAMETOGAME_API AC_MasterCard : public AActor
{
    GENERATED_BODY()
    
public:	
    // Sets default values for this actor's properties
    AC_MasterCard();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:	
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
