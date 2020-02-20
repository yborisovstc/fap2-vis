
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

AVCpsCp::AVCpsCp(const string& aName, MUnit* aMan, MEnv* aEnv): Unit(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_WidgetCpName);
    InsertContent(KCont_CtrUri);
}

string AVCpsCp::PEType()
{
    return Unit::PEType() + GUri::KParentSep + Type();
}

// TODO Why we need separate instance for each widget input? To have just one CP for all inputs
void AVCpsCp::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    const string cpName = GetContent(KCont_WidgetCpName);
    //const string cntUri = GetContent(KCont_CtrUri);
    //MUnit* cnt = GetNode(cntUri);
    MUnit* cnt = host->GetMan();
    if (cnt != NULL) {
	TIfRange rr;
	TICacheRCtx ctx(aCtx); ctx.push_back(this);
	// Iterating thru slots
	for (auto ind = 0; ind < cnt->CompsCount(); ind++) {
	    MUnit* compu = cnt->GetComp(ind);
	    MVCslot* comps = compu->GetObj(comps);
	    if (comps != NULL) {
		if (compu->CompsCount() == 1) {
		    MUnit* widget = compu->GetComp(0);
		    MUnit* outp = widget->GetNode("./" + cpName);
		    if (outp != NULL) {
			rr = outp->GetIfi(aName, ctx);
			InsertIfCache(aName, aCtx, outp, rr);
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
}


// Container


const string KWidgetRqsW_Name = "OutRqsW";
const string KWidgetRqsH_Name = "OutRqsH";

AVContainer::AVContainer(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
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
	    }
	    // Downward request, redirect to comp
	}
    } else {
	AVWidget::UpdateIfi(aName, aCtx);
    }
#if 0
    if (aCtx.empty() || IsComp(aCtx.back())) {
	// Upward request, redirect to owner
	rr = host->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, host, rr);
    } else {
	// Downward request, redirect to comp
	for (auto ind = 0; ind < CompsCount(); ind++) {
	    MUnit* comp = GetComp(ind);
	    rr = comp->GetIfi(aName, ctx);
	    InsertIfCache(aName, aCtx, comp, rr);
	}
    }
#endif
}

void AVContainer::Update()
{
    AVWidget::Update();
}

void AVContainer::Render()
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
    TBool res = ETrue;
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

int AVContainer::GetRqs(const string& aSlot, bool aW)
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

/*
   AVContainer::WidgetAp::WidgetAp(AVContainer& aHost, const string& aSlot): mHost(aHost), mSlot(aSlot), mAlcW(this, E_AlcW), mAlcH(this, E_AlcH) {
   mAlcH.SetHost(this);
}
*/

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



// Horizontal layout

const string KSlot_Name = "Slot";
const string KSlot_1_Name = "Slot_1";

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
    TBool res = ETrue;
    return res;
}

/** @brief Notifies widgets of inp updated
 * param[inp] aOutUri container "output" assosiated to widget input
 * */
void AVHLayout::NotifyWidgetOnInpUpdated(const string& aOutUri)
{
    MUnit* alcOut = GetNode(aOutUri);
    __ASSERT(alcOut != NULL);
    TIfRange rr = alcOut->GetIfi(MDesInpObserver::Type(), this);
    for (auto it = rr.first; it != rr.second; it++) {
	MDesInpObserver* mobs = (MDesInpObserver*) (*it);
	mobs->OnInpUpdated();
    }
}

void AVHLayout::OnInpUpdated()
{
    Logger()->Write(EInfo, this, "OnUpdated");
    MUnit* rqsInpW = GetNode("./RqsWInp");
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
    /*
    MUnit* alcWOut = GetNode("./AlcWOut");
    __ASSERT(alcWOut != NULL);
    rr = alcWOut->GetIfi(MDesInpObserver::Type(), this);
    for (auto it = rr.first; it != rr.second; it++) {
	MDesInpObserver* mobs = (MDesInpObserver*) (*it);
	mobs->OnInpUpdated();
    }
    */
    NotifyWidgetOnInpUpdated("./AlcWOut");
    NotifyWidgetOnInpUpdated("./AlcHOut");
    NotifyWidgetOnInpUpdated("./AlcXOut");
    NotifyWidgetOnInpUpdated("./AlcYOut");
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
	    res = x + w + 1;
	}
    } else if (aPar == E_AlcY) {
    } else if (aPar == E_AlcW) {
	res = GetRqs(aSlotName, true);
    } else if (aPar == E_AlcH) {
	res = GetRqs(aSlotName, false);
    }
    return res;
}

int AVHLayout::GetSlotId(const string& aSlotName) const
{
    int res = -1;
    size_t sp = aSlotName.find_last_of('_');
    string id = aSlotName.substr(sp + 1);
    res = stoi(id);
    return res;
}

string AVHLayout::GetSlotName(int aSlotId) const
{
    return KSlot_Name + "_" + to_string(aSlotId);
}
