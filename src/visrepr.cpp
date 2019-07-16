
#include "visrepr.h"


AVisRepr::AVisRepr(const string& aName, MUnit* aMan, MEnv* aEnv): Unit(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVisRepr::PEType()
{
    return Unit::PEType() + GUri::KParentSep + Type();
}

MIface *AVisRepr::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVisRepr::Type()) == 0) {
	res = dynamic_cast<MVisRepr*>(this);
    } else {
	res = Unit::DoGetObj(aName);
    }
    return res;
}


MIface* AVisRepr::MAgent_DoGetIface(const string& aUid)
{
    MIface* res = NULL;
    if (aUid == MVisRepr::Type())
	res = dynamic_cast<MVisRepr*>(this);
    return res;
}

MIfProv* AVisRepr::IfProv()
{
    MIfProv* res = NULL;
    return res;
}

