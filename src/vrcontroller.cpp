
#include <mprov.h>
#include <env.h>
#include <mdes.h>

#include "vrcontroller.h"
#include "magentvr.h"



const string KVrc_ModelPath = "";

/** @brief Content: scene URI */
const string KVrc_Scene = "Scene";

AVrController::AVrController(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mEnv(NULL)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KVrc_ModelPath);
    InsertContent(KVrc_Scene);
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
    const string sceneUri = iMan->GetContent(KVrc_Scene);
    MUnit* sceneu = GetNode(sceneUri);
    for (int ind = 0; ind < sceneu->CompsCount(); ind++) {
	MUnit* comp = sceneu->GetComp(ind);
	MVrp* drp = (MVrp*) comp->GetSIfi(MVrp::Type());
	if (drp != nullptr) {
	    res = comp; break;
	}
    }
    return res;
}

MVrp* AVrController::GetDrp()
{
    MVrp* res = nullptr;
    const string sceneUri = iMan->GetContent(KVrc_Scene);
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
    const string sceneUri = iMan->GetContent(KVrc_Scene);
    MUnit* sceneu = GetNode(sceneUri);
    __ASSERT(sceneu);
    MElem* scenee = sceneu->GetObj(scenee);
    __ASSERT(scenee);
    string drpType = "UnitDrp";
    scenee->AppendMutation(TMut(ENt_Node, ENa_Targ, sceneUri , ENa_Id, aNode->Name(), ENa_Parent, "/*/Modules/AvrMdl/" + drpType));
    TNs ns; MutCtx mctx(NULL, ns);
    scenee->Mutate(true, false, false, mctx);
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
    return res;
}

MElem* AVrController::GetSceneE()
{
    const string sceneUri = iMan->GetContent(KVrc_Scene);
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
	} else if (aContName == KVrc_Scene) {
	    res = true;
	}
    }
    return res;
}

void AVrController::Update()
{
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
    MVrp* drp = CreateDrp(mEnv->Root());
}

void AVrController::OnRpSelected(const MVrp* aRp)
{
    string mdlUri = aRp->GetModel();
    MUnit* mdl = mEnv->Root()->GetNode(mdlUri);
    __ASSERT(mdl);
    MUnit* drp = GetDrpU();
    __ASSERT(drp);
    MElem* scenee = GetSceneE();
    __ASSERT(scenee);
    //string drpUri = drp->GetUri(NULL, true);
    scenee->AppendMutation(TMut(ENt_Rm, ENa_MutNode, "./" + drp->Name()));
    TNs ns; MutCtx mctx(NULL, ns);
    scenee->Mutate(true, false, false, mctx);
    CreateDrp(mdl);
}




TrModelCreated::TrModelCreated(const string& aName, MUnit* aMan, MEnv* aEnv): ATrcBase(aName, aMan, aEnv)
{
    iName = aName.empty() ? ATrcBase::GetType(PEType()) : aName;
    TBool res = mOut->ChangeCont("{Provided:'MDVarGet'}");
    __ASSERT(res);
    Unit* cp = Provider()->CreateNode("ConnPointMcu", "Inp", this, iEnv);
    __ASSERT(cp != NULL);
    res = AppendComp(cp);
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
    if (strcmp(aName, MDVarGet::Type()) == 0) {
	res = dynamic_cast<MDVarGet*>(this);
    } else if (strcmp(aName, MDtGet<Sdata<bool>>::Type()) == 0) {
	res = dynamic_cast<MDtGet<Sdata<bool>>*>(this);
    } else {
	res = ATrcBase::DoGetObj(aName);
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
	Logger()->Write(EInfo, this, "Module path changed: %s", modelPath.mData.c_str());
	MVrController* ctr = dynamic_cast<MVrController*>(iMan->GetSIfi(MVrController::Type()));
	__ASSERT(ctr);
	ctr->CreateModel(modelPath.mData);
    }
    aData.mData = res;

}
