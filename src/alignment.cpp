
#include "mwindow.h"
#include "alignment.h"


AAlignment::AAlignment(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainer(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AAlignment::PEType()
{
    return AVContainer::PEType() + GUri::KParentSep + Type();
}

void AAlignment::Render()
{
    AVContainer::Render();
}

void AAlignment::Init()
{
    AVContainer::Init();
}

TBool AAlignment::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = AVContainer::HandleCompChanged(aContext, aComp, aContName);
    return res;
}

int AAlignment::GetComposedData(const string& aSlotName, TWdgPar aPar)
{
    int res = 0;
    int wndWidth = 0, wndHeight = 0;
    Wnd()->GetFbSize(&wndWidth, &wndHeight);
    if (wndWidth >0 && wndHeight > 0) {
	if (aPar == E_AlcX) {
	    res = mPadding;
	} else if (aPar == E_AlcY) {
	    res = mPadding;
	} else if (aPar == E_AlcW) {
	    res = GetCompRqs(aSlotName, true);
	} else if (aPar == E_AlcH) {
	    res = GetCompRqs(aSlotName, false);
	}
    }
    return res;
}

