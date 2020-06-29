
#include "vlayoutl.h"

// ==== AVLayoutL ====

const string KSlotType = "/*/Modules/ContainerModL/FVLayoutLSlot";

AVLayoutL::AVLayoutL(const string& aName, MUnit* aMan, MEnv* aEnv): ALinearLayout(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVLayoutL::PEType()
{
    return ALinearLayout::PEType() + GUri::KParentSep + Type();
}

string AVLayoutL::GetSlotType()
{
    return KSlotType;
}
