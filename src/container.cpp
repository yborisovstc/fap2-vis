
#include "mprov.h"

#include "container.h"

// Container's slot

AVSlot::AVSlot(const string& aName, MUnit* aMan, MEnv* aEnv): Vertu(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVSlot::PEType()
{
    return Vertu::PEType() + GUri::KParentSep + Type();
}

void AVSlot::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    bool done = false;
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    if (aCtx.empty() || IsComp(aCtx.back())) {
	// Upward request, redirect to owner
	MUnit* agt = host->GetNode("./CntAgent");
	rr = agt->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, host, rr);
    } else {
	// Downward request, redirect to comp
	for (auto ind = 0; ind < CompsCount(); ind++) {
	    MUnit* comp = GetComp(ind);
	    rr = comp->GetIfi(aName, ctx);
	    InsertIfCache(aName, aCtx, comp, rr);
	}
    }
}

MIface* AVSlot::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVCslot::Type()) == 0) {
	res = dynamic_cast<MVCslot*>(this);
    } else {
	res = Vertu::DoGetObj(aName);
    }
    return res;
}

// Container's compositor input

const string KCont_WidgetCpName = "WcpName";
const string KCont_CtrUri = "ContainerUri";

AVCpsCpInp::AVCpsCpInp(const string& aName, MUnit* aMan, MEnv* aEnv): ConnPointMcu(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_WidgetCpName);
    InsertContent(KCont_CtrUri);
    ChangeCont("{Provided:'MDVarGet' Required:'MDesInpObserver'}");
}

string AVCpsCpInp::PEType()
{
    return ConnPointMcu::PEType() + GUri::KParentSep + Type();
}

void AVCpsCpInp::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    MUnit* host = iMan;
    const string cpName = GetContent(KCont_WidgetCpName);
    // Handle local first
    MIface* res = Base::GetObj(aName.c_str());
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    } else if (aName == MDVarGet::Type()) {
	MUnit* cnt = host;
	if (cnt != NULL) {
	    // Iterating thru slots
	    for (auto ind = 0; ind < cnt->CompsCount(); ind++) {
		MUnit* compu = cnt->GetComp(ind);
		MVCslot* comps = compu->GetObj(comps);
		if (comps != NULL) {
		    if (compu->CompsCount() == 1) {
			MUnit* widget = compu->GetComp(0);
			MUnit* outp = widget->GetNode("./" + cpName);
			if (outp != NULL) {
			    if (!ctx.IsInContext(outp)) {
				rr = outp->GetIfi(aName, ctx);
				InsertIfCache(aName, aCtx, outp, rr);
			    }
			} else {
			    Logger()->Write(EErr, this, "Cannot find widget output [%s]", cpName.c_str());
			}
		    } else {
			Logger()->Write(EErr, this, "Slot components != 1");
		    }
		}
	    }
	} else {
	    Logger()->Write(EErr, this, "Cannot find container");
	}
    } else if (aName == MDesInpObserver::Type()) {
	// Base behavior
	ConnPointMcu::UpdateIfi(aName, aCtx);
    } else {
	ConnPointMcu::UpdateIfi(aName, aCtx);
    }
}


AVCpsCpOut::AVCpsCpOut(const string& aName, MUnit* aMan, MEnv* aEnv): ConnPointMcu(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_WidgetCpName);
    InsertContent(KCont_CtrUri);
    ChangeCont("{Required:'MDVarGet' Provided:'MDesInpObserver'}");
}

string AVCpsCpOut::PEType()
{
    return ConnPointMcu::PEType() + GUri::KParentSep + Type();
}

void AVCpsCpOut::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    MUnit* host = iMan;
    const string cpName = GetContent(KCont_WidgetCpName);
    // Handle local first
    MIface* res = Base::GetObj(aName.c_str());
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    } else if (aName == MDesInpObserver::Type()) {
	MUnit* cnt = host;
	if (cnt != NULL) {
	    // Iterating thru slots
	    for (auto ind = 0; ind < cnt->CompsCount(); ind++) {
		MUnit* compu = cnt->GetComp(ind);
		MVCslot* comps = compu->GetObj(comps);
		if (comps != NULL) {
		    if (compu->CompsCount() == 1) {
			MUnit* widget = compu->GetComp(0);
			MUnit* outp = widget->GetNode("./" + cpName);
			if (outp != NULL) {
			    if (!ctx.IsInContext(outp)) {
				rr = outp->GetIfi(aName, ctx);
				InsertIfCache(aName, aCtx, outp, rr);
			    }
			} else {
			    Logger()->Write(EErr, this, "Cannot find widget output [%s]", cpName.c_str());
			}
		    } else {
			Logger()->Write(EErr, this, "Slot components != 1");
		    }
		}
	    }
	} else {
	    Logger()->Write(EErr, this, "Cannot find container");
	}
    } else if (aName == MDesInpObserver::Type()) {
	// Base behavior
	ConnPointMcu::UpdateIfi(aName, aCtx);
    } else {
	ConnPointMcu::UpdateIfi(aName, aCtx);
    }
}




// Transition of container requistion state

TrCntReq::TrCntReq(const string& aName, MUnit* aMan, MEnv* aEnv): ATrcBase(aName, aMan, aEnv)
{
    iName = aName.empty() ? ATrcBase::GetType(PEType()) : aName;
    TBool res = mOut->ChangeCont("{Provided:'MDVarGet' Required:'MDesInpObserver'}");
    __ASSERT(res);
    Unit* cp = Provider()->CreateNode("ConnPointMcu", "Inp", this, iEnv);
    __ASSERT(cp != NULL);
    res = AppendComp(cp);
    __ASSERT(res);
    res = cp->ChangeCont("{Provided:'MDesInpObserver' Required:'MDVarGet'}");
    __ASSERT(res);
    // Padding input
    Unit* padd_cp = Provider()->CreateNode("ConnPointMcu", "Padding", this, iEnv);
    __ASSERT(padd_cp != NULL);
    res = AppendComp(padd_cp);
    __ASSERT(res);
    res = padd_cp->ChangeCont("{Provided:'MDesInpObserver' Required:'MDVarGet'}");
    __ASSERT(res);
}

string TrCntReq::PEType()
{
    return ATrcBase::PEType() + GUri::KParentSep + Type();
}

MIface* TrCntReq::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDVarGet::Type()) == 0) {
	res = dynamic_cast<MDVarGet*>(this);
    } else if (strcmp(aName, MDtGet<Sdata<int>>::Type()) == 0) {
	res = dynamic_cast<MDtGet<Sdata<int>>*>(this);
    } else {
	res = ATrcBase::DoGetObj(aName);
    }
    return res;
}

void* TrCntReq::DoGetDObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<int>>::Type()) == 0) res = dynamic_cast<MDtGet<Sdata<int>>*>(this);
    return res;
}


// Transition of container requistion state: Sum

TrReqSum::TrReqSum(const string& aName, MUnit* aMan, MEnv* aEnv): TrCntReq(aName, aMan, aEnv)
{
    iName = aName.empty() ? Unit::GetType(PEType()) : aName;
}

string TrReqSum::PEType()
{
    return TrCntReq::PEType() + GUri::KParentSep + Type();
}

void TrReqSum::DtGet(Sdata<int>& aData)
{
    int res = 0;
    // Sum of inputs data
    MUnit* inp = GetNode("./Inp");
    __ASSERT(inp);
    TIfRange rr = inp->GetIfi(MDVarGet::Type(), this);
    int count = 0;
    for (auto it = rr.first; it != rr.second; it++) {
	MDVarGet* dvg = (MDVarGet*) (*it);
	MDtGet<Sdata<int> >* dg = dvg->GetDObj(dg);
	if (dg != NULL) {
	    Sdata<int> arg;
	    dg->DtGet(arg);
	    res += arg.mData;
	}
	count++;
    }
    // Padding
    MUnit* pinp = GetNode("./Padding");
    __ASSERT(pinp);
    MDVarGet* pinpd = dynamic_cast<MDVarGet*>(pinp->GetSIfi(MDVarGet::Type(), this));
    if (pinpd) {
	MDtGet<Sdata<int> >* pinpdg = pinpd->GetDObj(pinpdg);
	if (pinpdg != NULL) {
	    Sdata<int> arg;
	    pinpdg->DtGet(arg);
	    res = res + arg.mData * (count + 1);
	}
    }
    aData.mData = res;
}


// Transition of container requistion state: Max

TrReqMax::TrReqMax(const string& aName, MUnit* aMan, MEnv* aEnv): TrCntReq(aName, aMan, aEnv)
{
    iName = aName.empty() ? Unit::GetType(PEType()) : aName;
}

string TrReqMax::PEType()
{
    return TrCntReq::PEType() + GUri::KParentSep + Type();
}

void TrReqMax::DtGet(Sdata<int>& aData)
{
    int res = 0;
    MUnit* inp = GetNode("./Inp");
    __ASSERT(inp);
    TIfRange rr = inp->GetIfi(MDVarGet::Type(), this);
    for (auto it = rr.first; it != rr.second; it++) {
	MDVarGet* dvg = (MDVarGet*) (*it);
	MDtGet<Sdata<int> >* dg = dvg->GetDObj(dg);
	if (dg != NULL) {
	    Sdata<int> arg;
	    dg->DtGet(arg);
	    res = max(res, arg.mData);
	}
    }
    // Padding
    MUnit* pinp = GetNode("./Padding");
    __ASSERT(pinp);
    MDVarGet* pinpd = dynamic_cast<MDVarGet*>(pinp->GetSIfi(MDVarGet::Type(), this));
    if (pinpd) {
	MDtGet<Sdata<int> >* pinpdg = pinpd->GetDObj(pinpdg);
	if (pinpdg != NULL) {
	    Sdata<int> arg;
	    pinpdg->DtGet(arg);
	    res = res + arg.mData * 2;
	}
    }
    aData.mData = res;
}




// Container

const string KSlot_Name = "Slot";
const string KRqW_Uri = "./RqsW";
const string KRqH_Uri = "./RqsH";
const string KRqWInp_Uri = KRqW_Uri + "/Inp" ;
const string KRqHInp_Uri = KRqH_Uri + "/Inp" ;
const string KCompRqWInp_Uri = "./RqsWInp";
const string KCompRqHInp_Uri = "./RqsHInp";

const string KWidgetRqsW_Name = "OutRqsW";
const string KWidgetRqsH_Name = "OutRqsH";
const string KCont_Padding = "Padding";

AVContainer::AVContainer(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Padding);
}

string AVContainer::PEType()
{
    return AVWidget::PEType() + GUri::KParentSep + Type();
}

MIface* AVContainer::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MSceneElem::Type()) == 0) {
	res = dynamic_cast<MSceneElem*>(this);
    } else if (strcmp(aName, MDesInpObserver::Type()) == 0) {
	res = dynamic_cast<MDesInpObserver*>(this);
    } else {
	res = AVWidget::DoGetObj(aName);
    }
    return res;
}

void AVContainer::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    if (aName == MDVarGet::Type()) {
	if (!aCtx.empty()) {
	    MUnit* rqs = aCtx.back(); // Slot
	    MVCslot* rqss = rqs->GetObj(rqss);
	    if (rqss != NULL) {
		string slotName = rqs->Name();
		if (mWap.count(slotName) == 0) {
		    mWap.emplace(slotName, WidgetAp(*this, slotName));
		}
		MUnit* irqs = aCtx.front();
		WidgetAp& wap = mWap.at(slotName);
		WidgetAp::WdgPap* pap = NULL;
		if (irqs->Name() == "AlcW") {
		    pap = &(wap.mAlcW);
		} else if (irqs->Name() == "AlcH") {
		    pap = &(wap.mAlcH);
		} else if (irqs->Name() == "AlcX") {
		    pap = &(wap.mAlcX);
		} else if (irqs->Name() == "AlcY") {
		    pap = &(wap.mAlcY);
		}
		if (pap != NULL) {
		    MDVarGet* papdvg = dynamic_cast<MDVarGet*>(pap);
		    InsertIfCache(aName, aCtx, this, papdvg);
		}
	    } else if (rqs == GetRqsInp(true)) {
		// Request from W requisition
		MIface* r = dynamic_cast<MDVarGet*>(&mTrRecW);
		InsertIfCache(aName, aCtx, this, r);
	    } else if (rqs == GetRqsInp(false)) {
		// Request from H requisition
		MIface* r = dynamic_cast<MDVarGet*>(&mTrRecH);
		InsertIfCache(aName, aCtx, this, r);
	    }
	    // Downward request, redirect to comp
	}
    } else if (aName == MSceneElem::Type()) {
	// Enable local request only to isolate the whole tree layers
	if (aCtx.empty() || (aCtx.back() == this)) {
	    for (int ind = 0; ind < host->CompsCount(); ind++) {
		MUnit* compu = host->GetComp(ind);
		MVCslot* comps = compu->GetObj(comps);
		if (comps != NULL) {
		    if (compu->CompsCount() == 1) {
			MUnit* wdg = compu->GetComp(0);
			rr = wdg->GetIfi(aName, ctx);
			InsertIfCache(aName, aCtx, wdg, rr);
		    }
		}
	    }
	} else {
	    AVWidget::UpdateIfi(aName, aCtx);
	}
    } else if (aName == MDesInpObserver::Type()) {
	if (!aCtx.empty()) {
	    // Bind component's requisition outputs to container requisition states
	    MUnit* rqs = aCtx.back(); // Slot
	    MVCslot* rqss = rqs->GetObj(rqss);
	    if (rqss != NULL) {
		if (rqs->CompsCount() == 1) {
		    MUnit* wdg = rqs->GetComp(0);
		    MUnit* rwout = wdg->GetNode("./OutRqsW");
		    MUnit* rhout = wdg->GetNode("./OutRqsH");
		    MUnit* prov = NULL;
		    if (aCtx.IsInContext(rwout)) {
			prov = GetCompRqsInp(true);
		    } else if (aCtx.IsInContext(rhout)) {
			prov = GetCompRqsInp(false);
		    }
		    rr = prov->GetIfi(aName, ctx);
		    InsertIfCache(aName, aCtx, prov, rr);
		}
	    }
	}
    } else {
	AVWidget::UpdateIfi(aName, aCtx);
    }
}

void AVContainer::Update()
{
    AVWidget::Update();
}

void AVContainer::Render()
{

    TIfRange rr = GetIfi(MSceneElem::Type());
    for (auto it = rr.first; it != rr.second; it++) {
	MSceneElem* se = dynamic_cast<MSceneElem*>(*it);
	se->Render();
    }
}
/*
{
    MUnit* host = GetMan();
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* compu = host->GetComp(ind);
	MVCslot* comps = compu->GetObj(comps);
	if (comps != NULL) {
	    if (compu->CompsCount() == 1) {
		MUnit* wdg = compu->GetComp(0);
		MSceneElem* mse = (MSceneElem*) wdg->GetSIfi(MSceneElem::Type());
		if (mse != NULL) {
		    mse->Render();
		}
	    }
	}
    }

}
*/

void AVContainer::Init()
{
    AVWidget::Init();
    // Set up widget requisition representations 
    // Iterating thru slots
    MUnit* host = iMan;
    for (auto ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* compu = host->GetComp(ind);
	MVCslot* comps = compu->GetObj(comps);
	if (comps != NULL) {
	    if (compu->CompsCount() == 1) {
		MUnit* widget = compu->GetComp(0);
		// Width
		MUnit* outp = widget->GetNode("./" + KWidgetRqsW_Name );
		if (outp != NULL) {
		    MDVarGet* outpd = (MDVarGet*) outp->GetSIfi(MDVarGet::Type(), this);
		    mRqsW.emplace(compu->Name(), outpd);
		} else {
		    Logger()->Write(EErr, this, "Cannot find widget output [%s]", KWidgetRqsW_Name.c_str());
		}
		// Hight
		outp = widget->GetNode("./" + KWidgetRqsH_Name );
		if (outp != NULL) {
		    MDVarGet* outpd = (MDVarGet*) outp->GetSIfi(MDVarGet::Type(), this);
		    mRqsH.emplace(compu->Name(), outpd);
		} else {
		    Logger()->Write(EErr, this, "Cannot find widget output [%s]", KWidgetRqsH_Name.c_str());
		}
	    } else {
		Logger()->Write(EErr, this, "Slot components != 1");
	    }
	}
    }
}

TBool AVContainer::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = EFalse;
    if (&aComp == GetMan() && aContName == KCont_Padding) {
	const string data = aComp.GetContent(KCont_Padding);
	mPadding = stoi(data);
	res = ETrue;
    }
    return res;
}


MIface* AVContainer::MDesInpObserver_Call(const string& aSpec, string& aRes)
{
    return NULL;
}

string AVContainer::MDesInpObserver_Mid() const
{
    return GetUri(NULL, ETrue);
}

MUnit* AVContainer::GetCompRqsInp(bool aW)
{
    MUnit* res = NULL;
    res = Host()->GetNode(aW ? KCompRqWInp_Uri : KCompRqHInp_Uri);
    return res;
}

MUnit* AVContainer::GetRqsInp(bool aW)
{
    MUnit* res = NULL;
    res = Host()->GetNode(aW ? KRqWInp_Uri: KRqHInp_Uri);
    return res;
}

int AVContainer::GetCompRqs(const string& aSlot, bool aW)
{
    int res = 0;
    TWDvgProv& reg = (aW ? mRqsW : mRqsH);    
    if (reg.count(aSlot) > 0) {
	MDVarGet* dvg = (aW ? mRqsW : mRqsH).at(aSlot);
	MDtGet<Sdata<int> >* dg = dvg->GetDObj(dg);
	if (dg != NULL) {
	    Sdata<int> arg;
	    dg->DtGet(arg);
	    res = arg.mData;
	}
    }
    return res;
}

void* AVContainer::WidgetAp::WdgPap::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (aName == MDtGet<Sdata<int> >::Type()) {
	res = dynamic_cast<MDtGet<Sdata<int> >*>(this);
    }
    return res;
}

void AVContainer::WidgetAp::WdgPap::DtGet(Sdata<int>& aData)
{
    int res = mHost->mHost.GetComposedData(mHost->mSlot, mPar);
    aData.Set(res);
}


// Transition of requisition

void* AVContainer::TrReq::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (aName == MDtGet<Sdata<int> >::Type()) {
	res = dynamic_cast<MDtGet<Sdata<int> >*>(this);
    }
    return res;
}

void AVContainer::TrReq::DtGet(Sdata<int>& aData)
{
    int res = mHost->mPadding;
    MUnit* inp = mHost->GetCompRqsInp(mW);
    __ASSERT(inp);
    TIfRange rr = inp->GetIfi(MDVarGet::Type());
    for (auto it = rr.first; it != rr.second; it++) {
	MDVarGet* dvg = dynamic_cast<MDVarGet*>(*it);
	MDtGet<Sdata<int> >* dg = dvg->GetDObj(dg);
	int idata = 0;
	if (dg != NULL) {
	    Sdata<int> arg;
	    dg->DtGet(arg);
	    idata = arg.mData;
	}
	res += idata + mHost->mPadding;
    }
    aData.Set(res);
}




void AVContainer::onSeCursorPosition(double aX, double aY)
{
    TIfRange rr = GetIfi(MSceneElem::Type());
    for (auto it = rr.first; it != rr.second; it++) {
	MSceneElem* se = dynamic_cast<MSceneElem*>(*it);
	se->onSeCursorPosition(aX, aY);
    }
}

bool AVContainer::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    bool res = false;
    TIfRange rr = GetIfi(MSceneElem::Type());
    // Redirect to comps till the event gets accepted
    for (auto it = rr.first; it != rr.second; it++) {
	MSceneElem* se = dynamic_cast<MSceneElem*>(*it);
	res = se->onMouseButton(aButton, aAction, aMods);
	if (res) break;
    }
    return res;
}

MUnit* AVContainer::GetCntComp(const string& aCompName)
{
    MUnit* res = NULL;
    MUnit* host = GetMan();
    string uri = "./" + aCompName;
    // Uri with "any" elem is not supported atm, so using search by slots
    for (int ind = 0; ind < host->CompsCount() && res == nullptr; ind++) {
	MUnit* compu = host->GetComp(ind);
	MVCslot* comps = compu->GetObj(comps);
	if (comps != nullptr) {
	    res = compu->GetNode(uri);
	}
    }
    return res;
}

string AVContainer::GetSlotName(int aSlotId) const
{
    return KSlot_Name + "_" + to_string(aSlotId);
}

int AVContainer::GetSlotId(const string& aSlotName) const
{
    int res = -1;
    size_t sp = aSlotName.find_last_of('_');
    string id = aSlotName.substr(sp + 1);
    res = stoi(id);
    return res;
}

int AVContainer::GetNextSlot(int aSlotId) const
{
    return aSlotId + 1;
}

int AVContainer::GetLastSlot()
{
    MUnit* host = GetMan();
    int lastSlotId = 0;
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* compu = host->GetComp(ind);
	MVCslot* comps = compu->GetObj(comps);
	if (comps != nullptr) {
	    int slotId = GetSlotId(compu->Name());
	    lastSlotId = max(lastSlotId, slotId);
	}
    }
    return lastSlotId;
}

void AVContainer::AddComp(const string& aName, const string& aType, const string& aHint)
{
    MUnit* host = GetMan();
    int lastSlotId = GetLastSlot();
    // Add new slot first
    MElem* hoste = host->GetObj(hoste);
    __ASSERT(hoste);
    string newSlotName = GetSlotName(lastSlotId + 1);
    hoste->AppendMutation(TMut(ENt_Node, ENa_Id, newSlotName, ENa_Parent, "AVSlot"));
    TNs ns; MutCtx mctx(NULL, ns);
    hoste->Mutate(true, false, false, mctx);
    string newSlotUri = "./" + newSlotName;
    MUnit* newSlot = host->GetNode(newSlotUri);
    __ASSERT(newSlot);
    hoste->AppendMutation(TMut(ENt_Node, ENa_Targ, newSlotUri , ENa_Id, aName, ENa_Parent, "/*/Modules/FvWidgets/" + aType));
    hoste->Mutate(true, false, false, mctx);
    string newWdgUri = newSlotUri + "/" + aName;
    MUnit* newWdg = host->GetNode(newWdgUri);
    __ASSERT(newWdg);
    // Invalidate Iface cache
    InvalidateIfCache();
}

void AVContainer::NotifyWidgetOnInpUpdated(const string& aOutUri)
{
    MUnit* alcOut = Host()->GetNode(aOutUri);
    __ASSERT(alcOut != NULL);
    TIfRange rr = alcOut->GetIfi(MDesInpObserver::Type(), this);
    for (auto it = rr.first; it != rr.second; it++) {
	MDesInpObserver* mobs = (MDesInpObserver*) (*it);
	mobs->OnInpUpdated();
    }
}

void AVContainer::NotifyReqsOnInpUpdated(bool aW)
{
    MUnit* rqinp = GetRqsInp(aW); 
    __ASSERT(rqinp);
    MDesInpObserver* rqinpo = dynamic_cast<MDesInpObserver*>(rqinp->GetSIfi(MDesInpObserver::Type()));
    __ASSERT(rqinpo);
    rqinpo->OnInpUpdated();
}

void AVContainer::OnInpUpdated()
{
    Logger()->Write(EInfo, this, "OnInpUpdated");
    MUnit* rqsInpW = GetCompRqsInp(true);
    __ASSERT(rqsInpW != NULL);
    TIfRange rr = rqsInpW->GetIfi(MDVarGet::Type(), this);
    for (auto it = rr.first; it != rr.second; it++) {
	MDVarGet* dvg = (MDVarGet*) (*it);
	MDtGet<Sdata<int> >* dg = dvg->GetDObj(dg);
	if (dg != NULL) {
	    Sdata<int> arg;
	    dg->DtGet(arg);
	    string dis;
	    arg.ToString(dis);
	    Logger()->Write(EInfo, this, "Requisition W: %s", dis.c_str());
	}
    }
    // Propagate update notification to inp observers
    NotifyReqsOnInpUpdated(true);
    NotifyReqsOnInpUpdated(false);
    NotifyWidgetOnInpUpdated("./AlcWOut");
    NotifyWidgetOnInpUpdated("./AlcHOut");
    NotifyWidgetOnInpUpdated("./AlcXOut");
    NotifyWidgetOnInpUpdated("./AlcYOut");
}




// Horizontal layout


AVHLayout::AVHLayout(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainer(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVHLayout::PEType()
{
    return AVContainer::PEType() + GUri::KParentSep + Type();
}

MIface* AVHLayout::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MSceneElem::Type()) == 0) {
	res = dynamic_cast<MSceneElem*>(this);
    } else {
	res = AVContainer::DoGetObj(aName);
    }
    return res;
}

void AVHLayout::Update()
{
    AVContainer::Update();
}

void AVHLayout::Render()
{
    AVContainer::Render();
}

void AVHLayout::Init()
{
    AVContainer::Init();
}

TBool AVHLayout::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = AVContainer::HandleCompChanged(aContext, aComp, aContName);
    return res;
}

int AVHLayout::GetComposedData(const string& aSlotName, TWdgPar aPar)
{
    int res = 0;
    if (aPar == E_AlcX) {
	int cid = GetSlotId(aSlotName);
	if (cid > 1) {
	    int pid = cid - 1;
	    string psn = GetSlotName(pid);
	    int x = GetComposedData(psn, E_AlcX);
	    int w = GetComposedData(psn, E_AlcW);
	    res = x + w + 2*mPadding;
	} else {
	    res = mPadding;
	}
    } else if (aPar == E_AlcY) {
	res = mPadding;
    } else if (aPar == E_AlcW) {
	res = GetCompRqs(aSlotName, true);
    } else if (aPar == E_AlcH) {
	res = GetCompRqs(aSlotName, false);
    }
    return res;
}

