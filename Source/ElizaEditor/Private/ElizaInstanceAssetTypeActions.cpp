// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#include "ElizaInstanceAssetTypeActions.h"
#include "ElizaInstance.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

FElizaInstanceAssetTypeActions::FElizaInstanceAssetTypeActions(EAssetTypeCategories::Type Category)
    : MyAssetCategory(Category)
{
}

UClass* FElizaInstanceAssetTypeActions::GetSupportedClass() const
{
    return UElizaInstance::StaticClass();
}

FText FElizaInstanceAssetTypeActions::GetName() const
{
    return INVTEXT("Eliza Instance");
}

FColor FElizaInstanceAssetTypeActions::GetTypeColor() const
{
    return FColor::Cyan;
}

uint32 FElizaInstanceAssetTypeActions::GetCategories()
{
    return MyAssetCategory;
}

bool FElizaInstanceAssetTypeActions::CanLocalize() const
{
    return false;
}
