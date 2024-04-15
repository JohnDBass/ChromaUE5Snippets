
#include "ProjectUnrest/Tools/BlueprintAssertLibrary.h"


void UBlueprintAssertLibrary::AssertIsTrue(UObject* BlueprintCaller, const bool Condition)
{
	//Avoid throwing exceptions in Shipping or Test builds unless explicitly enabled
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST) || USE_LOGGING_IN_SHIPPING 
	if (!Condition)
	{
		FBlueprintExceptionInfo BreakpointExceptionInfo(EBlueprintExceptionType::Breakpoint);
		FBlueprintCoreDelegates::ThrowScriptException(
			BlueprintCaller,
			*(FBlueprintContextTracker::Get().GetCurrentScriptStackWritable().Last()),
			BreakpointExceptionInfo
		);
	}
#endif

}

void UBlueprintAssertLibrary::AssertIsFalse(UObject* BlueprintCaller, const bool Condition)
{
	AssertIsTrue(BlueprintCaller, !Condition);
}

void UBlueprintAssertLibrary::AssertIsNull(UObject* BlueprintCaller, UObject* RefToCheck)
{
	if (RefToCheck != nullptr)
		AssertIsTrue(BlueprintCaller, false);
}

void UBlueprintAssertLibrary::AssertIsNotNull(UObject* BlueprintCaller, UObject* RefToCheck)
{
	if (RefToCheck == nullptr)
		AssertIsTrue(BlueprintCaller, false);
}
