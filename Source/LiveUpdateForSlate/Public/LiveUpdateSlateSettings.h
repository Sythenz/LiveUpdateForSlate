// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LiveUpdateSlateSettings.generated.h"

/**
 * 
 */

UCLASS(config = EditorPerProjectUserSettings)
class LIVEUPDATEFORSLATE_API ULiveUpdateSlateSettings : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category = RHI, meta = (DisplayName = "Enable Live Update for Slate", ToolTip = "Refreshes the Editor's Slate Layout when Live Coding patches are complete."))
	bool bEnableLiveUpdateForSlate = true;
};
