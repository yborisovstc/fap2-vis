
#include "mwindow.h"
#include "alignment.h"

const string KSlotName = "Slot";
const string KSlotCpName = "SCp";
const string KWidgetCpName = "Cp";

AAlignment::AAlignment(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainerL(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AAlignment::PEType()
{
    return AVContainerL::PEType() + GUri::KParentSep + Type();
}

TBool AAlignment::HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName)
{
    TBool res = AVContainerL::HandleCompChanged(aContext, aComp, aContName);
    return res;
}

MUnit* AAlignment::AddWidget(const string& aName, const string& aType, const string& aHint)
{
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    __ASSERT(hoste);
    // No need to add new slot, it is already there
    string slotUri = "./" + KSlotName;
    MUnit* slot = host->GetNode(slotUri);
    __ASSERT(slot);
    // Add new widget
    TNs ns; MutCtx mctx(NULL, ns);
    hoste->AppendMutation(TMut(ENt_Node, ENa_Id, aName, ENa_Parent, aType));
    hoste->Mutate(true, false, false, mctx);
    string newWdgUri = "./" + aName;
    MUnit* newWdg = host->GetNode(newWdgUri);
    __ASSERT(newWdg);
    // Bind widget to slot
    string widgetCp = "./" + aName + "/" + KWidgetCpName;
    string slotCp = slot->GetUri(this, ETrue);
    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, widgetCp, ENa_Q, slotCp + "/" + KSlotCpName));
    hoste->Mutate(true, false, false, mctx);
    // Invalidate Iface cache
    InvalidateIfCache();
    return newWdg;
}

MUnit* AAlignment::GetSlotByPos(const TPos& aPos)
{
    MUnit* res = nullptr;
    if (aPos == KPosFirst) {
	MUnit* host = GetMan();
	res = host->GetNode("./" + KSlotName);
    }
    return res;
}

void AAlignment::UpdateCompNames()
{
    mCompNames.mData.clear();
    MUnit* slot = GetSlotByPos(KPosFirst);
    if (slot) {
	MUnit* widget = GetWidgetBySlot(slot);
	if (widget) {
	    mCompNames.mData.push_back(widget->Name());
	}
    }
}
