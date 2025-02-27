// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "CoreMinimal.h"
#include "ISettingsModule.h"
#include "Interfaces/ITargetPlatformModule.h"
#include "Modules/ModuleManager.h"
#include "UObject/Package.h"
#include "UObject/WeakObjectPtr.h"
#include "ElizaPluginSettings.h"
#include "Styling/SlateStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#define LOCTEXT_NAMESPACE "ElizaEditorModule"

class FElizaEditorModule : public IModuleInterface
{
	
	virtual void StartupModule() override
	{

		//Create class icons
		StyleSet = MakeShareable(new FSlateStyleSet("ElizaAssetStyle"));
		FString ContentDir = IPluginManager::Get().FindPlugin("Eliza")->GetBaseDir();
		StyleSet->SetContentRoot(ContentDir);
		RegisterSettings();
	}

	virtual void ShutdownModule() override
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(StyleSet->GetStyleSetName());

		if (UObjectInitialized())
		{
			UnregisterSettings();
		}
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

private:
	TSharedPtr<FSlateStyleSet> StyleSet;
	bool HandleSettingsSaved()
	{
		UElizaPluginSettings* Settings = GetMutableDefault<UElizaPluginSettings>();
		bool ResaveSettings = false;

		if (ResaveSettings)
		{
			Settings->SaveConfig();
		}

#if (ENGINE_MINOR_VERSION >= 1) && (ENGINE_MAJOR_VERSION >= 5)
		bool AllowLibcurlConnectionReuse = false;
		if (GConfig) {
			GConfig->GetBool(TEXT("/Script/ElizaEditor.ElizaPluginSettings"), TEXT("AllowLibcurlConnectionReuse"), AllowLibcurlConnectionReuse, GGameIni);
			GConfig->SetBool(TEXT("HTTP.Curl"), TEXT("bForbidReuse"), !(AllowLibcurlConnectionReuse), GEngineIni);
			GConfig->Flush(false, GEngineIni);
		}
#endif

		return true;
	}

	void RegisterSettings()
	{

		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

			// Register the settings
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "Eliza",
				LOCTEXT("RuntimeEditorSettingsName", "Eliza"),
				LOCTEXT("RuntimeEditorSettingsDescription", "Settings for the Eliza plugin."),
				GetMutableDefault<UElizaPluginSettings>()
				);


			if (SettingsSection.IsValid())
			{
				SettingsSection->OnModified().BindRaw(this, &FElizaEditorModule::HandleSettingsSaved);
			}
		}
	}

	void UnregisterSettings()
	{

		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			SettingsModule->UnregisterSettings("Project", "ElizaEditor", "Eliza");
		}
	}
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE( FElizaEditorModule, ElizaEditor );