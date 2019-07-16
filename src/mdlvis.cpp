
#include "mdlvis.h"


AMdlVis::AMdlVis(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AMdlVis::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AMdlVis::Construct()
{
}

MIface *AMdlVis::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MMdlVis::Type()) == 0) {
	res = dynamic_cast<MMdlVis*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

MIfProv* AMdlVis::IfProv()
{
    MIfProv* res = NULL;
    return res;
}
