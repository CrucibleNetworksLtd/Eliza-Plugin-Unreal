// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#include "CoreMinimal.h"
#include "Interfaces/ITargetPlatformModule.h"
#include "Modules/ModuleManager.h"
#include "UObject/Package.h"
#include "UObject/WeakObjectPtr.h"
#include "Styling/SlateStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "AssetTypeCategories.h"
#include "AssetToolsModule.h"
#include "ElizaInstanceFactory.h"
#include "ElizaInstanceAssetTypeActions.h"
#define LOCTEXT_NAMESPACE "ElizaEditorModule"

class FElizaEditorModule : public IModuleInterface
{
private:
	TSharedPtr<FElizaInstanceAssetTypeActions> ElizaAssetTypeActions;
public:
	virtual void StartupModule() override
	{

		//Create class icons
		StyleSet = MakeShareable(new FSlateStyleSet("ElizaAssetStyle"));
		FString ContentDir = IPluginManager::Get().FindPlugin("Eliza")->GetBaseDir();
		StyleSet->SetContentRoot(ContentDir);
		
		FSlateImageBrush* ElizaInstanceThumbnailBrush = new FSlateImageBrush(StyleSet->RootToContentDir(TEXT("Resources/ElizaInstance128"), TEXT(".png")), FVector2D(128.0f, 128.0f));
		if (ElizaInstanceThumbnailBrush) {
			StyleSet->Set("ClassThumbnail.ElizaInstance", ElizaInstanceThumbnailBrush);
			FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
		}
		
		//Create web3 category
		EAssetTypeCategories::Type AgentsCategory = FAssetToolsModule::GetModule().Get().RegisterAdvancedAssetCategory(FName(TEXT("Agents")), FText::FromString("Agents"));
		
		//Register asset types
		ElizaAssetTypeActions = MakeShareable(new FElizaInstanceAssetTypeActions(AgentsCategory));
		FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ElizaAssetTypeActions.ToSharedRef());
	}

	virtual void ShutdownModule() override
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

private:
	TSharedPtr<FSlateStyleSet> StyleSet;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FElizaEditorModule, ElizaEditor );