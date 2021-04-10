#include "C_PlayerController.h"


#include "EngineUtils.h"
#include "Components/ScrollBox.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"


AC_PlayerController::AC_PlayerController(const FObjectInitializer& ObjectInitializer)
{
	const auto DialogListBPPath = TEXT("/Game/GameCore/Widgets/WBP_CharacterDialogText");
	static ConstructorHelpers::FClassFinder<UUserWidget> TextLineWidget(DialogListBPPath);
	if (ensure(TextLineWidget.Class != nullptr))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 // FString::Printf(TEXT("The widget is found %s"), DialogListBPPath));
		TextLineWidgetClass = TextLineWidget.Class;
		UE_LOG(LogTemp, Log, TEXT("The widget class \"%s\"has been found in \"%s\""), *TextLineWidgetClass->GetName(), DialogListBPPath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find the widget class in \"%s\""), DialogListBPPath);
		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 // FString::Printf(TEXT("Unable to find %s"), DialogListBPPath));
	}
}

void AC_PlayerController::TEST_EVENT_Implementation(float val)
{
	// GetWorld()->GetAuthGameMode()->FindComponentByClass()

	// for (TActorIterator<UScrollBox> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	// {
	// 	if (ActorItr->GetName() == "Dialog_ScrollBox")
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Wow"));
	// 		// ActorItr->Slate(true);
	// 	}
	// }

	auto ScrollBox = FindObject<UScrollBox>(ANY_PACKAGE, TEXT("Dialog_ScrollBox"));
	if (!ScrollBox)
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Ah :("));
	else
	{
		auto cur_val = ScrollBox->AllowOverscroll;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Settings allow overscroll %d -> %d"), cur_val, !cur_val));
		ScrollBox->SetAllowOverscroll(!cur_val);


		// See further: https://youtu.be/XKNIQga3HYk
		// WidgetT* CreateWidget(OwnerT* OwningObject, TSubclassOf<UUserWidget> UserWidgetClass = WidgetT::StaticClass(), FName WidgetName = NAME_None)
		// static int32_t line_number = 0;
		// auto* text_line = CreateWidget<UUserWidget>(ScrollBox, this->TextLineWidgetClass, FString::Printf(TEXT("DialogLine_%d"), line_number));
		// // text_line->AddToViewport();
		// ScrollBox->AddChild(text_line);
	}


	// GetWorld()->FindObjec
}

TSubclassOf<class UObject> AC_PlayerController::FindOrLoadBluePrintClass(const TCHAR* path)
{
	// finds or loads a blueprint class. Needed to create BP based objects in C++.
	/*
	    // usage:
	    TSubclassOf<class UObject> BP_Item3DActor= FindOrLoadBluePrintClass(TEXT("/Game/FirstPerson/Blueprints/BP_Item3DActor"));

	    AItem3DActor* t = Common::world->SpawnActor<AItem3DActor>(BP_Item3DActor, wPos, SpawnRotation, SpawnParams);
	    // 't' actually points to a BP class derived from C++ class 'AItem3DActor'
	*/

	UObject* something = StaticLoadObject(UObject::StaticClass(), nullptr, path);
	UBlueprint* bp = Cast<UBlueprint>(something);
	TSubclassOf<class UObject> MyItemBlueprint;
	MyItemBlueprint = (UClass*)bp->GeneratedClass;

	return MyItemBlueprint;
}
