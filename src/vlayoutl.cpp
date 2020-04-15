
#include "vlayoutl.h"

// ==== AVLayoutL ====

AVLayoutL::AVLayoutL(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainerL(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVLayoutL::PEType()
{
    return AVContainerL::PEType() + GUri::KParentSep + Type();
}


