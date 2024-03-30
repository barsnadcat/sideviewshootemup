#include "SideViewShootemupEditor.h"

#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"

IMPLEMENT_GAME_MODULE(FSideViewShootemupEditorModule, SideViewShootemupEditor);


DEFINE_LOG_CATEGORY(GameEditor)

#define LOCTEXT_NAMESPACE "GameEditor"

void FSideViewShootemupEditorModule::StartupModule()
{
    UE_LOG(GameEditor, Display, TEXT("GameEditor: Log Started"));
}

void FSideViewShootemupEditorModule::ShutdownModule()
{
    UE_LOG(GameEditor, Display, TEXT("GameEditor: Log Ended"));
}

#undef LOCTEXT_NAMESPACE
