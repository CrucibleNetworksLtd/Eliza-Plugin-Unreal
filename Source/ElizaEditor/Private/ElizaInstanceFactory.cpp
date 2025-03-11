// Copyright Crucible Networks Ltd 2023. All Rights Reserved.


#include "ElizaInstanceFactory.h"
#include "ElizaInstance.h"

UElizaInstanceFactory::UElizaInstanceFactory() {
    SupportedClass = UElizaInstance::StaticClass();
    bCreateNew = true;
}

UObject* UElizaInstanceFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
    return NewObject<UElizaInstance>(InParent, Class, Name, Flags, Context);
}
