#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "C_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMETOGAME_API AC_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AC_PlayerController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void TEST_EVENT(const float val);

private:
	TSubclassOf<UObject> FindOrLoadBluePrintClass(const TCHAR* path);
	TSubclassOf<UUserWidget> TextLineWidgetClass;
};
