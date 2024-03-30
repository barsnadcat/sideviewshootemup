#include "SideViewShootemupEditor.h"

#include "AssetToolsModule.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "ShipDesignActions.h"

IMPLEMENT_GAME_MODULE(FSideViewShootemupEditorModule, SideViewShootemupEditor);

DEFINE_LOG_CATEGORY(GameEditor)

#define LOCTEXT_NAMESPACE "GameEditor"

void FSideViewShootemupEditorModule::StartupModule()
{
    UE_LOG(GameEditor, Display, TEXT("StartupModule"));
    ShipDesignActions = MakeShared<FShipDesignActions>();
    FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ShipDesignActions.ToSharedRef());
}

void FSideViewShootemupEditorModule::ShutdownModule()
{
    UE_LOG(GameEditor, Display, TEXT("ShutdownModule"));

    if (!FModuleManager::Get().IsModuleLoaded("AssetTools"))
        return;
    FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(ShipDesignActions.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
