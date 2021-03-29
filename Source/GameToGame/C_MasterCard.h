// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_MasterCard.generated.h"


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


public: // Overrides
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
