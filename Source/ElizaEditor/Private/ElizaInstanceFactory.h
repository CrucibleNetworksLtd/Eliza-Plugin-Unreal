// Copyright Crucible Networks Ltd 2023. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "ElizaInstanceFactory.generated.h"

/**
 * 
 */
UCLASS()
class ELIZAEDITOR_API UElizaInstanceFactory : public UFactory
{
	GENERATED_BODY()
public:
    UElizaInstanceFactory();
    UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn);
};
