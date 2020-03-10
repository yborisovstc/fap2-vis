
#include "vlayout.h"


AVLayout::AVLayout(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainer(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVLayout::PEType()
{
    return AVContainer::PEType() + GUri::KParentSep + Type();
}

void AVLayout::Render()
{
    AVContainer::Render();
}

void AVLayout::Init()
{
    AVContainer::Init();
}

TBool AVLayout::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = AVContainer::HandleCompChanged(aContext, aComp, aContName);
    return res;
}

int AVLayout::GetComposedData(const string& aSlotName, TWdgPar aPar)
{
    int res = 0;
    if (aPar == E_AlcX) {
	res = mPadding;
    } else if (aPar == E_AlcY) {
	int cid = GetSlotId(aSlotName);
	if (cid > 1) {
	    int pid = cid - 1;
	    string psn = GetSlotName(pid);
	    int y = GetComposedData(psn, E_AlcY);
	    int h = GetComposedData(psn, E_AlcH);
	    res = y + h + 2*mPadding;
	} else {
	    res = mPadding;
	}
    } else if (aPar == E_AlcW) {
	res = GetRqs(aSlotName, true);
    } else if (aPar == E_AlcH) {
	res = GetRqs(aSlotName, false);
    }
    return res;
}

