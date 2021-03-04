
#include "containerl.h"

const string KWidgetRqsW_Name = "OutRqsW";
const string KWidgetRqsH_Name = "OutRqsH";
const string KCont_Padding = "Padding";
const string KWidgetCpName = "Cp";
const string KSlotCpName = "SCp";
const string KSlot_Name = "Slot";

const string K_CpUriInpAddWidget = "./../InpMutAddWidget";
const string K_CpUriInpRmWidget = "./../InpMutRmWidget";
const string K_CpUriCompNames = "./../OutCompNames";
const string K_CpUriCompCount = "./../OutCompsCount";

const MContainer::TPos AVContainerL::KPosFirst = TPos(0, 0);
const MContainer::TPos AVContainerL::KPosEnd = TPos(-1, -1);

AVContainerL::AVContainerL(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv), mMag(aMan),
    mMutRmWidget(-1)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Padding);
    iEnv->SetObserver(&mMagObs);
}

AVContainerL::~AVContainerL()
{
    iEnv->UnsetObserver(&mMagObs);
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
    } else if (aName == MDVarGet::Type()) {
	MUnit* cmpCount = GetNode(K_CpUriCompCount);
	if (ctx.IsInContext(cmpCount)) {
	    MIface* iface = dynamic_cast<MDVarGet*>(&mApCmpCount);
	    InsertIfCache(aName, aCtx, cmpCount, iface);
	}
	MUnit* cmpNames = GetNode(K_CpUriCompNames);
	if (ctx.IsInContext(cmpNames)) {
	    MIface* iface = dynamic_cast<MDVarGet*>(&mApCmpNames);
	    InsertIfCache(aName, aCtx, cmpNames, iface);
	}
    } else if (aName == MDesInpObserver::Type()) {
	MUnit* mutAddWdt = GetNode(K_CpUriInpAddWidget);
	if (ctx.IsInContext(mutAddWdt)) {
	    MIface* iface = dynamic_cast<MDesInpObserver*>(&mIapMutAddWdt);
	    InsertIfCache(aName, aCtx, mutAddWdt, iface);
	} else {
	    MUnit* mutRmWdt = GetNode(K_CpUriInpRmWidget);
	    if (ctx.IsInContext(mutRmWdt)) {
		MIface* iface = dynamic_cast<MDesInpObserver*>(&mIapMutRmWdt);
		InsertIfCache(aName, aCtx, mutRmWdt, iface);
	    }
	}
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

MUnit* AVContainerL::AddWidget(const string& aName, const string& aType, const string& aHint)
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
    hoste->AppendMutation(TMut(ENt_Node, ENa_Id, aName, ENa_Parent, aType));
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
    return newWdg;
}

bool AVContainerL::RmWidget(int aSlotPos, const string& aHint)
{
    bool res = false;
    Logger()->Write(EInfo, this, "Start removing widget on slot [%d]", aSlotPos);
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    __ASSERT(hoste);
    MUnit* slot = GetSlotByPos(TPos(aSlotPos,0));
    if (slot) {
	MUnit* widget = GetWidgetBySlot(slot);
	if (widget) {
	    // Remove widget
	    string wname = widget->Name();
	    hoste->AppendMutation(TMut(ENt_Rm, ENa_MutNode, wname));
	    TNs ns; MutCtx mctx(NULL, ns);
	    hoste->Mutate(true, false, false, mctx);
	    Logger()->Write(EInfo, this, "Completed removing widget");
	} else {
	    Logger()->Write(EErr, this, "Cannot find widget in slot [%d]", aSlotPos);
	}
    } else {
	Logger()->Write(EErr, this, "Cannot find slot [%d]", aSlotPos);
    }
    return res;
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

MUnit* AVContainerL::InsertWidget(const string& aName, const string& aType, const TPos& aPos)
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
    hoste->AppendMutation(TMut(ENt_Node, ENa_Id, aName, ENa_Parent, aType));
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
    InsertSlot(newSlot, aPos);
    // Invalidate Iface cache
    InvalidateIfCache();
    return newWdg;
}

MContainer::TPos AVContainerL::LastPos() const
{
    __ASSERT(false);
}

MUnit* AVContainerL::GetPrevSlotCp(MUnit* aSlot)
{
    __ASSERT(false);
}

MUnit* AVContainerL::GetNextSlotCp(MUnit* aSlot)
{
    __ASSERT(false);
}

MUnit* AVContainerL::GetNextSlot(MUnit* aSlot)
{
    __ASSERT(false);
}

void AVContainerL::RmSlot(MUnit* aSlot)
{
    __ASSERT(false);
}


MUnit* AVContainerL::InsertSlot(MUnit* aSlot, const TPos& aPos)
{
    __ASSERT(false);
}

MUnit* AVContainerL::GetSlotByPos(const TPos& aPos)
{
    __ASSERT(false);
}

MContainer::TPos AVContainerL::PrevPos(const TPos& aPos) const
{
    __ASSERT(false);
}

MContainer::TPos AVContainerL::NextPos(const TPos& aPos) const
{
    __ASSERT(false);
}

MUnit* AVContainerL::GetWidgetBySlot(MUnit* aSlot)
{
    MUnit* res = nullptr;
    MUnit* slotCp = aSlot->GetNode("./" + KSlotCpName);
    __ASSERT(slotCp);
    MVert* slotCpv = slotCp->GetObj(slotCpv);
    __ASSERT(slotCpv && (slotCpv->PairsCount() == 1));
    MVert* wdgCp = slotCpv->GetPair(0);
    __ASSERT(wdgCp);
    MUnit* wdgCpu = wdgCp->GetObj(wdgCpu);
    __ASSERT(wdgCpu);
    res = wdgCpu->GetMan();
    return res;
}

void AVContainerL::OnMutAddWdg()
{
    // Checking MutAddWidget
    MUnit* inp = GetNode(K_CpUriInpAddWidget);
    if (inp) {
	NTuple data;
	TBool res = GetGData(inp, data);
	if (res && data != mMutAddWidget) {
	    MutAddWidget(data);
	}
    }
}

void AVContainerL::OnMutRmWdg()
{
    // Checking MutRmWidget input
    MUnit* inp = GetNode(K_CpUriInpRmWidget);
    if (inp) {
	Sdata<int> data;
	TBool res = GetGData(inp, data);
	if (res && data != mMutRmWidget) {
	    MutRmWidget(data);
	}
    }
}


void AVContainerL::MutAddWidget(const NTuple& aData)
{
    TBool res = ETrue;
    string name;
    string type;
    for (TInt i = 0; i < aData.mData.size(); i++) {
	NTuple::tComp item = aData.mData.at(i);
	if (item.first == "name") {
	    Sdata<string>* sd = dynamic_cast<Sdata<string>*>(item.second);
	    if (sd) {
		name = sd->mData;
	    } else {
		res = EFalse; break;
	    }
	} else if (item.first == "type") {
	    Sdata<string>* sd = dynamic_cast<Sdata<string>*>(item.second);
	    if (sd) {
		type = sd->mData;
	    } else {
		res = EFalse; break;
	    }
	}
    }
    if (res) {
	AddWidget(name, type);
    }
}

void AVContainerL::MutRmWidget(const Sdata<int>& aData)
{
    int slotId = aData.mData;
    // Remove assosiated widget
    RmWidget(slotId);
    // Remove slot
    MUnit* curSlot = GetSlotByPos(TPos(slotId, 0));
    RmSlot(curSlot);
}

void AVContainerL::GetCompsCount(Sdata<TInt>& aData)
{
    aData.mData = mCompNames.mData.size();
    aData.mValid = ETrue;
}

void AVContainerL::OnMagCompDeleting(const MUnit* aComp, TBool aSoft, TBool aModif)
{
    if (aComp == mMag) {
	mCompNamesUpdated = ETrue;
    }
}

void AVContainerL::OnMagCompAdding(const MUnit* aComp, TBool aModif)
{
    if (mMag && mMag->IsComp(aComp)) {
	mCompNamesUpdated = ETrue;
	SetUpdated();
    }
}

TBool AVContainerL::OnMagCompChanged(const MUnit* aComp, const string& aContName, TBool aModif)
{
    return ETrue;
}

TBool AVContainerL::OnMagChanged(const MUnit* aComp)
{
    return ETrue;
}

TBool AVContainerL::OnMagCompRenamed(const MUnit* aComp, const string& aOldName)
{
    mCompNamesUpdated = ETrue;
    return ETrue;
}

void AVContainerL::OnMagCompMutated(const MUnit* aNode)
{
}

void AVContainerL::OnMagError(const MUnit* aComp)
{
}

void AVContainerL::Confirm()
{
    if (mMag) {
	if (mCompNamesUpdated) {
	    UpdateCompNames();
	    mCompNamesUpdated = EFalse;
	    MUnit* cp = GetNode(K_CpUriCompNames);
	    NotifyInpsUpdated(cp);
	    // Comps count
	    cp = GetNode(K_CpUriCompCount);
	    NotifyInpsUpdated(cp);
	}
    } else {
	Logger()->Write(EErr, this, "Managed agent is not attached");
    }
    AVWidget::Confirm();
}

void AVContainerL::NotifyInpsUpdated(MUnit* aCp)
{
    TIfRange range = aCp->GetIfi(MDesInpObserver::Type());
    for (TIfIter it = range.first; it != range.second; it++) {
	MDesInpObserver* mobs = (MDesInpObserver*) (*it);
	if (mobs != NULL) {
	    mobs->OnInpUpdated();
	}
    }
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


// ==== ALinearLayout ====

const string KStartSlotName = "Start";
const string KEndSlotName = "End";
const string KSlotPrevCpName = "Prev";
const string KSlotNextCpName = "Next";

ALinearLayout::ALinearLayout(const string& aName, MUnit* aMan, MEnv* aEnv): AVContainerL(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string ALinearLayout::PEType()
{
    return AVContainerL::PEType() + GUri::KParentSep + Type();
}

MUnit* ALinearLayout::GetLastSlot()
{
    MUnit* res = nullptr;
    MUnit* host = GetMan();
    MUnit* end = host->GetNode("./" + KEndSlotName);
    if (end) {
	MVert* endv = end->GetObj(endv);
	if (endv) {
	    MVert* lastv = endv->GetPair(0);
	    if (lastv) {
		MUnit* prevu = lastv->GetObj(prevu);
		if (prevu) {
		    res = prevu->GetMan();
		}
	    }
	}
    }
    return res;
}

MUnit* ALinearLayout::AppendSlot(MUnit* aSlot)
{
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    MUnit* lastSlot = GetLastSlot();
    MUnit* end = host->GetNode("./" + KEndSlotName);
    MUnit* start = host->GetNode("./" + KStartSlotName);
    __ASSERT(start && end);
    string endUri = end->GetUri(this, ETrue);
    string newSlotUri = aSlot->GetUri(this, ETrue);
    string newSlotNextUri = newSlotUri + "/" + KSlotNextCpName;
    string newSlotPrevUri = newSlotUri + "/" + KSlotPrevCpName;
    TNs ns; MutCtx mctx(NULL, ns);
    if (lastSlot) {
	// There are slots already
	// Disconnect last slot from end
	string lastSlotUri = lastSlot->GetUri(this, ETrue);
	string lastSlotPrevUri = lastSlotUri + "/" + KSlotPrevCpName;
	hoste->AppendMutation(TMut(ENt_Disconn, ENa_P, lastSlotPrevUri, ENa_Q, endUri));
	hoste->Mutate(true, false, false, mctx);
	// Connect new slot to last slot
	hoste->AppendMutation(TMut(ENt_Conn, ENa_P, lastSlotPrevUri, ENa_Q, newSlotNextUri));
	hoste->Mutate(true, false, false, mctx);
    } else {
	// There are no slots yet
	// Connect new slot to start
	string startUri = start->GetUri(this, ETrue);
	hoste->AppendMutation(TMut(ENt_Conn, ENa_P, startUri, ENa_Q, newSlotNextUri));
	hoste->Mutate(true, false, false, mctx);
    }
    // Connect new slot to end
    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, newSlotPrevUri, ENa_Q, endUri));
    hoste->Mutate(true, false, false, mctx);
    return NULL; // TODO to fix
}

void ALinearLayout::RmSlot(MUnit* aSlot)
{
    string sname = aSlot->Name();
    Logger()->Write(EInfo, this, "Start removing slot [%s]", sname.c_str());
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    // Get prev and next
    MUnit* prevSlotCp = GetPrevSlotCp(aSlot);
    string prevSlotUri = prevSlotCp->GetUri(this, ETrue);
    MUnit* nextSlotCp = GetNextSlotCp(aSlot);
    string nextSlotUri = nextSlotCp->GetUri(this, ETrue);
    // Remove slot, also disconnects from prev and next
    hoste->AppendMutation(TMut(ENt_Rm, ENa_MutNode, sname));
    TNs ns; MutCtx mctx(NULL, ns);
    hoste->Mutate(true, false, false, mctx);
    // Connect prev and next
    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, prevSlotUri, ENa_Q, nextSlotUri));
    hoste->Mutate(true, false, false, mctx);
    Logger()->Write(EInfo, this, "Completed removing slot");
}

MUnit* ALinearLayout::InsertSlot(MUnit* aSlot, const TPos& aPos)
{
    MUnit* host = GetMan();
    MElem* hoste = host->GetObj(hoste);
    MUnit* curSlot = GetSlotByPos(aPos);
    MUnit* end = host->GetNode("./" + KEndSlotName);
    MUnit* start = host->GetNode("./" + KStartSlotName);
    __ASSERT(start && end);
    string startUri = start->GetUri(this, ETrue);
    string endUri = end->GetUri(this, ETrue);
    string newSlotUri = aSlot->GetUri(this, ETrue);
    string newSlotNextUri = newSlotUri + "/" + KSlotNextCpName;
    string newSlotPrevUri = newSlotUri + "/" + KSlotPrevCpName;
    TNs ns; MutCtx mctx(NULL, ns);
    if (curSlot) {
	string curSlotUri = curSlot->GetUri(this, ETrue);
	string curSlotNextUri = curSlotUri + "/" + KSlotNextCpName;
	string curSlotPrevUri = curSlotUri + "/" + KSlotPrevCpName;
	MUnit* prevSlotCp = GetPrevSlotCp(curSlot);
	string prevSlotPrevUri = prevSlotCp->GetUri(this, ETrue);
	// Disconnect  cur slot from prev slot
	hoste->AppendMutation(TMut(ENt_Disconn, ENa_P, prevSlotPrevUri, ENa_Q, curSlotNextUri));
	// Connect new slot to prev slot
	hoste->AppendMutation(TMut(ENt_Conn, ENa_P, prevSlotPrevUri, ENa_Q, newSlotNextUri));
	// Connect new slot to cur slot
	hoste->AppendMutation(TMut(ENt_Conn, ENa_P, newSlotPrevUri, ENa_Q, curSlotNextUri));
	hoste->Mutate(true, false, false, mctx);
    } else {
	if (aPos == KPosFirst) {
	    // There is no slots yet
	    // Connect new slot to start
	    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, startUri, ENa_Q, newSlotNextUri));
	    // Connect new slot to end
	    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, newSlotPrevUri, ENa_Q, endUri));
	    hoste->Mutate(true, false, false, mctx);
	} else if (aPos == KPosEnd) {
	    MUnit* prevSlot = GetLastSlot();
	    string prevSlotPrevUri;
	    if (prevSlot) {
		prevSlotPrevUri = prevSlot->GetUri(this, ETrue) + "/" + KSlotPrevCpName;
	    } else {
		prevSlotPrevUri = startUri;
	    }
	    hoste->AppendMutation(TMut(ENt_Disconn, ENa_P, prevSlotPrevUri, ENa_Q, endUri));
	    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, prevSlotPrevUri, ENa_Q, newSlotNextUri));
	    hoste->AppendMutation(TMut(ENt_Conn, ENa_P, newSlotPrevUri, ENa_Q, endUri));
	    hoste->Mutate(true, false, false, mctx);
	} else {
	    __ASSERT(false);
	}
    }
    return NULL; // TODO to fix
}

MContainer::TPos ALinearLayout::PrevPos(const TPos& aPos) const
{
    __ASSERT(aPos != KPosFirst);
    TPos res = aPos;
    res.first--;
    return res;
}

MContainer::TPos ALinearLayout::NextPos(const TPos& aPos) const
{
    __ASSERT(aPos != KPosEnd);
    TPos res = aPos;
    res.first++;
    return res;
}

MUnit* ALinearLayout::GetSlotByPos(const TPos& aPos)
{
    MUnit* res = nullptr;
    if (aPos == KPosEnd) {
    } else if (aPos == KPosFirst) {
	MUnit* host = GetMan();
	MUnit* start = host->GetNode("./" + KStartSlotName);
	__ASSERT(start);
	MVert* startv = start->GetObj(startv);
	if (startv->PairsCount() == 1) {
	    MVert* first_next = startv->GetPair(0);
	    MUnit* first_nextu = first_next->GetObj(first_nextu);
	    res = first_nextu->GetMan();
	}
    } else {
	TPos pos = KPosFirst;
	MUnit* slot = GetSlotByPos(KPosFirst);
	while (pos != aPos) {
	    slot = GetNextSlot(slot);
	    pos = NextPos(pos);
	}
	res = slot;
    }
    return res;
}

MUnit* ALinearLayout::GetPrevSlotCp(MUnit* aSlot)
{
    MUnit* res = nullptr;
    MUnit* slot_nextu = aSlot->GetNode("./" + KSlotNextCpName);
    MVert* slot_nextv = slot_nextu->GetObj(slot_nextv);
    if (slot_nextv->PairsCount() == 1) {
	MVert* res_prev = slot_nextv->GetPair(0);
	res = res_prev->GetObj(res);
    }
    return res;
}

MUnit* ALinearLayout::GetNextSlotCp(MUnit* aSlot)
{
    MUnit* res = nullptr;
    MUnit* slot_prevu = aSlot->GetNode("./" + KSlotPrevCpName);
    MVert* slot_prevv = slot_prevu->GetObj(slot_prevv);
    if (slot_prevv->PairsCount() == 1) {
	MVert* res_next = slot_prevv->GetPair(0);
	res = res_next->GetObj(res);
    }
    return res;
}

MUnit* ALinearLayout::GetNextSlot(MUnit* aSlot)
{
    MUnit* res = nullptr;
    MUnit* ncp = GetNextSlotCp(aSlot);
    MUnit* nslu = ncp->GetMan();
    MVCslot* nsls = nslu->GetObj(nsls);
    if (nsls) {
	res = nslu;
    }
    return res;
}

void ALinearLayout::UpdateCompNames()
{
    mCompNames.mData.clear();
    MUnit* slot = GetSlotByPos(TPos(0,0));
    while (slot) {
	MUnit* widget = GetWidgetBySlot(slot);
	if (widget) {
	    mCompNames.mData.push_back(widget->Name());
	}
	slot = GetNextSlot(slot);
    }
}
