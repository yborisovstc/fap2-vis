
#include "containerl.h"

const string KWidgetRqsW_Name = "OutRqsW";
const string KWidgetRqsH_Name = "OutRqsH";
const string KCont_Padding = "Padding";
const string KWidgetCpName = "Cp";
const string KSlotCpName = "SCp";
const string KSlot_Name = "Slot";


AVContainerL::AVContainerL(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Padding);
}

string AVContainerL::PEType()
{
    return AVWidget::PEType() + GUri::KParentSep + Type();
}

MIface* AVContainerL::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MContainer::Type()) == 0) {
	res = dynamic_cast<MContainer*>(this);
    } else {
	res = AVWidget::DoGetObj(aName);
    }
    return res;
}

void AVContainerL::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    if (aName == MViewMgr::Type() && !aCtx.empty() && iMan->IsComp(aCtx.front())) {
	// For view manager redirect upward
	host = iMan->GetMan();
	rr = host->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, host, rr);
    } else {
	AVWidget::UpdateIfi(aName, aCtx);
    }
}

void AVContainerL::Render()
{
    Logger()->Write(EInfo, this, "Render");
    MUnit* host = GetMan();
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* comp = host->GetComp(ind);
	if (comp != this) {
	    MSceneElem* mse = comp->GetSIfit(mse);
	    if (mse) {
		mse->Render();
	    }
	}
    }
}

void AVContainerL::AddWidget(const string& aName, const string& aType, const string& aHint)
{
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    __ASSERT(hoste);
    // Add new slot
    string slotType = GetSlotType();
    __ASSERT(!slotType.empty());
    string slotName = GetSlotName(GetLastSlotId() + 1);
    hoste->AppendMutation(TMut(ENt_Node, ENa_Id, slotName, ENa_Parent, slotType));
    TNs ns; MutCtx mctx(NULL, ns);
    hoste->Mutate(true, false, false, mctx);
    string newSlotUri = "./" + slotName;
    MUnit* newSlot = host->GetNode(newSlotUri);
    __ASSERT(newSlot);
    // Add new widget
    hoste->AppendMutation(TMut(ENt_Node, ENa_Id, aName, ENa_Parent, "/*/Modules/FvWidgetsL/" + aType));
    hoste->Mutate(true, false, false, mctx);
    string newWdgUri = "./" + aName;
    MUnit* newWdg = host->GetNode(newWdgUri);
    __ASSERT(newWdg);
    // Bind widget to slot
    string widgetCp = "./" + aName + "/" + KWidgetCpName;
    string slotCp = newSlot->GetUri(this, ETrue);
    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, widgetCp, ENa_Q, slotCp + "/" + KSlotCpName));
    hoste->Mutate(true, false, false, mctx);
    // Append slot
    AppendSlot(newSlot);
    // Invalidate Iface cache
    InvalidateIfCache();
}

string AVContainerL::GetSlotType()
{
    return string();
}

MUnit* AVContainerL::AppendSlot(MUnit* aSlot)
{
    __ASSERT(false);
}

int AVContainerL::GetLastSlotId()
{
    MUnit* host = GetMan();
    int lastSlotId = 0;
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* compu = host->GetComp(ind);
	MVCslot* comps = compu->GetObj(comps);
	if (comps) {
	    int slotId = GetSlotId(compu->Name());
	    lastSlotId = max(lastSlotId, slotId);
	}
    }
    return lastSlotId;
}

int AVContainerL::GetSlotId(const string& aSlotName) const
{
    int res = -1;
    size_t sp = aSlotName.find_last_of('_');
    string id = aSlotName.substr(sp + 1);
    res = stoi(id);
    return res;
}

string AVContainerL::GetSlotName(int aSlotId) const
{
    return KSlot_Name + "_" + to_string(aSlotId);
}

bool AVContainerL::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    bool res = false;
    bool lres = AVWidget::onMouseButton(aButton, aAction, aMods);
    if (lres) {
	MUnit* host = GetMan();
	for (int ind = 0; ind < host->CompsCount() && !res; ind++) {
	    MUnit* comp = host->GetComp(ind);
	    if (comp != this) {
		MSceneElem* mse = comp->GetSIfit(mse);
		if (mse) {
		    res = mse->onMouseButton(aButton, aAction, aMods);
		}
	    }
	}
    }
    return res;
}


// ==== AVSlot ====

const string KCompRqWExtd_Uri = "./InpRqsW/Int";
const string KCompRqHExtd_Uri = "./InpRqsH/Int";

AVSlotL::AVSlotL(const string& aName, MUnit* aMan, MEnv* aEnv): Syst(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVSlotL::PEType()
{
    return Syst::PEType() + GUri::KParentSep + Type();
}

MIface* AVSlotL::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVCslot::Type()) == 0) {
	res = dynamic_cast<MVCslot*>(this);
    } else {
	res = Syst::DoGetObj(aName);
    }
    return res;
}
