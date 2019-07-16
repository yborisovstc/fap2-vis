
#include "scelem.h"


AScElem::AScElem(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AScElem::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AScElem::Update()
{
}

MIface *AScElem::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MSceneElem::Type()) == 0) {
	res = dynamic_cast<MSceneElem*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

