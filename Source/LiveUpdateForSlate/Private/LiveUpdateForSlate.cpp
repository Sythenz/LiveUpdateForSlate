// Copyright Epic Games, Inc. All Rights Reserved.

#include "LiveUpdateForSlate.h"
#include <Windows/LiveCoding/Public/ILiveCodingModule.h>
#include <MainFrame/Public/Interfaces/IMainFrameModule.h>
#include <Framework/Docking/TabManager.h>
#include <LiveUpdateSlateSettings.h>
#include <ISettingsModule.h>

#define LOCTEXT_NAMESPACE "FLiveUpdateSlateModule"

void FLiveUpdateForSlateModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		SettingsModule->RegisterSettings("Editor", "Plugins", "Slate Update for Live++",
			LOCTEXT("LiveUpdateSlateSettingsName", "Slate Update for Live++"),
			LOCTEXT("LiveUpdateSlateSettingsDescription", "Settings for Slate Update for Live++"),
			GetMutableDefault< ULiveUpdateSlateSettings >()
		);
	}

#if WITH_LIVE_CODING
	ILiveCodingModule* LiveCoding = FModuleManager::LoadModulePtr<ILiveCodingModule>(LIVE_CODING_MODULE_NAME);
	const bool LiveCodingEnabled = LiveCoding != nullptr && LiveCoding->IsEnabledForSession();


	if (LiveCodingEnabled)
	{
		LiveCoding->GetOnPatchCompleteDelegate().AddRaw(this, &FLiveUpdateForSlateModule::OnPatchComplete);
	}
#endif
}

void FLiveUpdateForSlateModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FLiveUpdateForSlateModule::OnPatchComplete()
{
	ULiveUpdateSlateSettings* Settings = GetMutableDefault<ULiveUpdateSlateSettings>();

	//Don't rebuild slate if the plugin is not enabled
	if (!Settings->bEnableLiveUpdateForSlate) return;

	//Save all open tabs
	FGlobalTabmanager::Get()->SaveAllVisualState();

	//Flush our config files
	const bool bRead = true;
	GConfig->Flush(bRead, GEditorLayoutIni);

	//Request the editor to rebuild Slate
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	MainFrameModule.RecreateDefaultMainFrame(false, false);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLiveUpdateForSlateModule, LiveUpdateForSlate)