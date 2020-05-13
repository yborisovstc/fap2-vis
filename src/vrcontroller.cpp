
#include <mprov.h>
#include <env.h>
#include <mdes.h>

#include "vrcontroller.h"
#include "magentvr.h"
#include "mcontainer.h"



const string KVrc_ModelPath = "";

/** @brief Content: DRP mounting point */
const string KVrc_DrpMp = "DrpMp";

AVrController::AVrController(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mEnv(NULL)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KVrc_ModelPath);
    InsertContent(KVrc_DrpMp);
}

AVrController::~AVrController()
{
    if (mEnv) {
	delete mEnv;
	mEnv = NULL;
    }
}

string AVrController::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

MIface* AVrController::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MACompsObserver::Type()) == 0) {
	res = dynamic_cast<MACompsObserver*>(this);
    } else if (strcmp(aName, MVrController::Type()) == 0) {
	res = dynamic_cast<MVrController*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

MUnit* AVrController::GetDrpU()
{
    MUnit* res = nullptr;
    MUnit* mpu = GetDrpMpU();
    for (int ind = 0; ind < mpu->CompsCount(); ind++) {
	MUnit* comp = mpu->GetComp(ind);
	MVrp* drp = (MVrp*) comp->GetSIfi(MVrp::Type());
	if (drp != nullptr) {
	    res = comp; break;
	}
    }
    return res;
}

string AVrController::GetDrpMpUri() const
{
    string cntUri = iMan->GetContent(KVrc_DrpMp);
    return cntUri;
}

MVrp* AVrController::GetDrp()
{
    MVrp* res = nullptr;
    const string sceneUri = GetDrpMpUri();
    MUnit* sceneu = GetNode(sceneUri);
    for (int ind = 0; ind < sceneu->CompsCount(); ind++) {
	MUnit* comp = sceneu->GetComp(ind);
	MVrp* drp = (MVrp*) comp->GetSIfi(MVrp::Type());
	if (drp != nullptr) {
	    res = drp; break;
	}
    }
    return res;
}

MVrp* AVrController::CreateDrp(const MUnit* aNode)
{
    MVrp* res = nullptr;
    MUnit* sceneu = GetDrpMpcU();
    __ASSERT(sceneu);
    MElem* scenee = sceneu->GetObj(scenee);
    __ASSERT(scenee);
    string drpType = "UnitDrp";
    const string drpmpUri = GetDrpMpUri();

    MContainer* mpc = sceneu->GetSIfit(mpc);
    __ASSERT(mpc);
    mpc->AddWidget(aNode->Name(), "/*/Modules/AvrMdl/UnitDrp");
#if 0
    scenee->AppendMutation(TMut(ENt_Node, ENa_Targ, drpmpUri , ENa_Id, aNode->Name(), ENa_Parent, "/*/Modules/AvrMdl/" + drpType));
    TNs ns; MutCtx mctx(NULL, ns);
    scenee->Mutate(true, false, false, mctx);
#endif
    // Activate scene, TODO to do it in Des::Mutate
    MDesSyncable* sceneds = dynamic_cast<MDesSyncable*>(sceneu->GetSIfi(MDesSyncable::Type()));
    __ASSERT(sceneds);
    sceneds->SetActive();
    // Set up created DRP
    MVrp* drp = GetDrp();
    assert(drp != nullptr);
    drp->SetEnv(mEnv);
    string mdlUri = aNode->GetUri(NULL, true); 
    drp->SetModel(mdlUri);
    // Bind model to root DRP
    drp->SetCrtlBinding(GetUri(NULL, true));
    Logger()->Write(EInfo, this, "Model cteated: %s", drpmpUri .c_str());
    return res;
}

MUnit* AVrController::GetDrpMpcU()
{
    MUnit* res = NULL;
    const string sceneUri = iMan->GetContent(KVrc_DrpMp);
    res = GetNode(sceneUri);
    return res;
}

MUnit* AVrController::GetDrpMpU()
{
    MUnit* res = NULL;
    const string mpUri = GetDrpMpUri();
    res = GetNode(mpUri);
    return res;
}

MElem* AVrController::GetDrpMpcE()
{
    const string sceneUri = iMan->GetContent(KVrc_DrpMp);
    MUnit* sceneu = GetNode(sceneUri);
    __ASSERT(sceneu);
    MElem* scenee = sceneu->GetObj(scenee);
    return scenee;
}

void AVrController::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    ADes::UpdateIfi(aName, aCtx);
#if 0
    MUnit* host = iMan;
    bool done = false;
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    if (aCtx.empty() || IsComp(aCtx.back())) {
	// Upward request, redirect to scene
	/*
	MUnit* drp = GetDrp();
	rr = drp->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, drp, rr);
	*/
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

TBool AVrController::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = EFalse;
    if (&aComp == GetMan()) {
	if (aContName == KVrc_ModelPath) {
	    string val = aComp.GetContent(KVrc_ModelPath);
	    res = true;
	} else if (aContName == KVrc_DrpMp) {
	    res = true;
	}
    }
    return res;
}

void AVrController::Update()
{
    Logger()->Write(EInfo, this, "Update");
    ADes::Update();
}

void AVrController::Init()
{
}

void AVrController::CreateModel(const string& aSpecPath)
{
    if (mEnv != nullptr) {
	delete mEnv;
	mEnv = nullptr;
    }
    // Create model
    mEnv = new Env(aSpecPath, aSpecPath + ".log");
    assert(mEnv != nullptr);
    mEnv->ConstructSystem();
    if (mEnv->Root() == NULL) {
	Logger()->Write(EErr, this, "Cannot create model [%s]", aSpecPath.c_str());
    }
}

void AVrController::OnRpSelected(const MVrp* aRp)
{
    string mdlUri = aRp->GetModel();
    MUnit* mdl = mEnv->Root()->GetNode(mdlUri);
    __ASSERT(mdl);
    MUnit* drp = GetDrpU();
    __ASSERT(drp);
    MElem* scenee = GetDrpMpcE();
    __ASSERT(scenee);
    //string drpUri = drp->GetUri(NULL, true);
    scenee->AppendMutation(TMut(ENt_Rm, ENa_MutNode, "./" + drp->Name()));
    TNs ns; MutCtx mctx(NULL, ns);
    scenee->Mutate(true, false, false, mctx);
    CreateDrp(mdl);
}

MUnit* AVrController::ModelRoot()
{
    MUnit* res = NULL;
    if (mEnv) {
	res = mEnv->Root();
    }
    return res;
}

void AVrController::ApplyCursor(const string& aCursor)
{
    MUnit* mdl = mEnv->Root()->GetNode(aCursor);
    __ASSERT(mdl);
    MUnit* drp = GetDrpU();
    if (drp) {
	// DRP already exists, remove
	MElem* scenee = GetDrpMpcE();
	__ASSERT(scenee);
	scenee->AppendMutation(TMut(ENt_Rm, ENa_MutNode, "./" + drp->Name()));
	TNs ns; MutCtx mctx(NULL, ns);
	scenee->Mutate(true, false, false, mctx);
    }
    CreateDrp(mdl);
}


// Transition

TrModelCreated::TrModelCreated(const string& aName, MUnit* aMan, MEnv* aEnv): ATrcVar(aName, aMan, aEnv)
{
    iName = aName.empty() ? ATrcBase::GetType(PEType()) : aName;
    Unit* cp = Provider()->CreateNode("ConnPointMcu", "Inp", this, iEnv);
    __ASSERT(cp != NULL);
    bool res = AppendComp(cp);
    __ASSERT(res);
    res = cp->ChangeCont("{Provided:'MDesInpObserver' Required:'MDVarGet'}");
    __ASSERT(res);
}

string TrModelCreated::PEType()
{
    return ATrcBase::PEType() + GUri::KParentSep + Type();
}

MIface* TrModelCreated::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<bool>>::Type()) == 0) {
	res = dynamic_cast<MDtGet<Sdata<bool>>*>(this);
    } else {
	res = ATrcVar::DoGetObj(aName);
    }
    return res;
}

void* TrModelCreated::DoGetDObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<bool>>::Type()) == 0) res = dynamic_cast<MDtGet<Sdata<bool>>*>(this);
    return res;
}

void TrModelCreated::DtGet(Sdata<bool>& aData)
{
    bool res = false;
    //MUnit* inp = GetNode("./Inp");
    MDVarGet* inpvg = (MDVarGet*) GetSIfi("./Inp", MDVarGet::Type());
    MDtGet<Sdata<string>>* dg = inpvg->GetDObj(dg);
    if (dg != NULL) {
	Sdata<string> modelPath;
	dg->DtGet(modelPath);
	Logger()->Write(EInfo, this, "Model path changed: %s", modelPath.mData.c_str());
	MVrController* ctr = dynamic_cast<MVrController*>(iMan->GetSIfi(MVrController::Type()));
	__ASSERT(ctr);
	ctr->CreateModel(modelPath.mData);
	res = true;
    }
    aData.mData = res;
}


// Transition of Cursor

TrCursor::TrCursor(const string& aName, MUnit* aMan, MEnv* aEnv): ATrcVar(aName, aMan, aEnv)
{
    iName = aName.empty() ? ATrcBase::GetType(PEType()) : aName;
    Unit* cp = Provider()->CreateNode("CpStatecInp", "InpCmdUp", this, iEnv);
    __ASSERT(cp != NULL);
    bool res = AppendComp(cp);
    __ASSERT(res);
    Unit* cpCursor = Provider()->CreateNode("CpStatecInp", "InpCursor", this, iEnv);
    __ASSERT(cpCursor != NULL);
    res = AppendComp(cpCursor);
    __ASSERT(res);
    Unit* cpMdlCreated = Provider()->CreateNode("CpStatecInp", "InpMdlCreated", this, iEnv);
    __ASSERT(cpMdlCreated != NULL);
    res = AppendComp(cpMdlCreated);
    __ASSERT(res);
    Unit* cpNodeSelected = Provider()->CreateNode("CpStatecInp", "InpNodeSelected", this, iEnv);
    __ASSERT(cpNodeSelected != NULL);
    res = AppendComp(cpNodeSelected);
    __ASSERT(res);
}

string TrCursor::PEType()
{
    return ATrcBase::PEType() + GUri::KParentSep + Type();
}

MIface* TrCursor::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<bool>>::Type()) == 0) {
	res = dynamic_cast<MDtGet<Sdata<string>>*>(this);
    } else {
	res = ATrcVar::DoGetObj(aName);
    }
    return res;
}

void* TrCursor::DoGetDObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<string>>::Type()) == 0) res = dynamic_cast<MDtGet<Sdata<string>>*>(this);
    return res;
}

void TrCursor::DtGet(Sdata<string>& aData)
{
    bool res = false;
    bool cmdUp = false;
    string cursor;
    string nodeSelected;
    bool mdlCreated = false;
    res = DtGetSdataInp(cmdUp, "InpCmdUp");
    res = res && DtGetSdataInp(cursor, "InpCursor");
    res = res && DtGetSdataInp(mdlCreated, "InpMdlCreated");
    res = res && DtGetSdataInp(nodeSelected, "InpNodeSelected");
    if (res) {
	GUri crsUri(cursor);
	if (crsUri.IsNil()) {
	    if (mdlCreated) {
		MVrController* ctr = dynamic_cast<MVrController*>(iMan->GetSIfi(MVrController::Type()));
		__ASSERT(ctr);
		MUnit* mdlRoot = ctr->ModelRoot();
		if (mdlRoot) {
		    string mdlRootUri = mdlRoot->GetUri(NULL, true);
		    aData = mdlRootUri;
		}
	    }
	} else { // crsUri.IsNil()
	    GUri nodeSelectedUri(nodeSelected);
	    if (!nodeSelectedUri.IsNil() && nodeSelected != cursor) {
		aData = nodeSelected;
	    } else if (cmdUp) {
		GUri cursorUri(cursor);
		cursorUri.RemoveLastElem();
		aData.mData = cursorUri;
	    }
	}
    } else {
	Logger()->Write(EErr, this, "Error getting inputs");
    }
}


// Transition of CursorApplied

TrCursorApplied::TrCursorApplied(const string& aName, MUnit* aMan, MEnv* aEnv): ATrcVar(aName, aMan, aEnv)
{
    iName = aName.empty() ? ATrcBase::GetType(PEType()) : aName;
    Unit* cp = Provider()->CreateNode("CpStatecInp", "Inp", this, iEnv);
    __ASSERT(cp != NULL);
    bool res = AppendComp(cp);
    __ASSERT(res);
}

string TrCursorApplied::PEType()
{
    return ATrcBase::PEType() + GUri::KParentSep + Type();
}

MIface* TrCursorApplied::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<bool>>::Type()) == 0) {
	res = dynamic_cast<MDtGet<Sdata<string>>*>(this);
    } else {
	res = ATrcVar::DoGetObj(aName);
    }
    return res;
}

void* TrCursorApplied::DoGetDObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MDtGet<Sdata<string>>::Type()) == 0) res = dynamic_cast<MDtGet<Sdata<string>>*>(this);
    return res;
}

void TrCursorApplied::DtGet(Sdata<string>& aData)
{
    bool res = false;
    string cursor;
    res = DtGetSdataInp(cursor, "Inp");
    GUri cursorUri(cursor);
    if (res && !cursorUri.IsNil()) {
	GUri crsUri(cursor);
	MVrController* ctr = iMan->GetSIfit(ctr);
	__ASSERT(ctr);
	ctr->ApplyCursor(cursor);
	aData = cursor;
    } else {
	Logger()->Write(EErr, this, "Error getting inputs");
    }
}
