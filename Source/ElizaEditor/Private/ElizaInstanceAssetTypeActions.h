// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "AssetTypeCategories.h"
/**
 * 
 */
class ELIZAEDITOR_API FElizaInstanceAssetTypeActions : public FAssetTypeActions_Base
{
public:
	FElizaInstanceAssetTypeActions(EAssetTypeCategories::Type Category);
	UClass* GetSupportedClass() const override;
	FText GetName() const override;
	FColor GetTypeColor() const override;
	uint32 GetCategories() override;
	bool CanLocalize() const override;
private:
	EAssetTypeCategories::Type MyAssetCategory;
};
