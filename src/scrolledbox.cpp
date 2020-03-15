
#include "mwindow.h"
#include "scrolledbox.h"


AScrBox::AScrBox(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainer(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AScrBox::PEType()
{
    return AVContainer::PEType() + GUri::KParentSep + Type();
}

void AScrBox::Render()
{
    AVContainer::Render();
}

void AScrBox::Init()
{
    AVContainer::Init();
}

TBool AScrBox::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = AVContainer::HandleCompChanged(aContext, aComp, aContName);
    return res;
}

int AScrBox::GetComposedData(const string& aSlotName, TWdgPar aPar)
{
    int res = 0;
    int wndWidth = 0, wndHeight = 0;
    Wnd()->GetFbSize(&wndWidth, &wndHeight);
    if (aPar == E_AlcX) {
	res = mPadding;
    } else if (aPar == E_AlcY) {
	int cid = GetSlotId(aSlotName);
	if (cid > 1) {
	    int pid = cid - 1;
	    string psn = GetSlotName(pid);
	    int y = GetComposedData(psn, E_AlcY);
	    int h = GetComposedData(psn, E_AlcH);
	    res = y - (h + 2*mPadding);
	} else {
	    int h = GetCompRqs(aSlotName, false);
	    res = wndHeight - (h + 2*mPadding);
	}
    } else if (aPar == E_AlcW) {
	res = GetCompRqs(aSlotName, true);
    } else if (aPar == E_AlcH) {
	res = GetCompRqs(aSlotName, false);
    }
    return res;
}

