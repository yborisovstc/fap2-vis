
#include "sce_rect.h"


ASceRect::ASceRect(const string& aName, MUnit* aMan, MEnv* aEnv): AScElem(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string ASceRect::PEType()
{
    return AScElem::PEType() + GUri::KParentSep + Type();
}

void ASceRect::Update()
{
}


void ASceRect::Render()
{
}
