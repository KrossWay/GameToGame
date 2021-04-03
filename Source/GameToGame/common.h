// Quick UE_LOG
#define ULOG(LEVEL, FORMAT, ...) UE_LOG(LogTemp, LEVEL, TEXT(FORMAT), __VA_ARGS__)
#define ULOG_FSTART UE_LOG(LogTemp, Log, TEXT("%s started."), *FString(__FUNCTION__))
#define ULOG_FFINISH UE_LOG(LogTemp, Log, TEXT("%s finished."), *FString(__FUNCTION__))
static const FString DATA_FOLDER_NAME("Data");

