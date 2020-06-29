
#include "hlayoutl.h"

const string KSlotType = "/*/Modules/ContainerModL/FHLayoutLSlot";

AHLayoutL::AHLayoutL(const string& aName, MUnit* aMan, MEnv* aEnv): ALinearLayout(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AHLayoutL::PEType()
{
    return ALinearLayout::PEType() + GUri::KParentSep + Type();
}

string AHLayoutL::GetSlotType()
{
    return KSlotType;
}
