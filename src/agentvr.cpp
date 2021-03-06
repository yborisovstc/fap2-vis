
#include <iostream> 
#include <FTGL/ftgl.h>

#include "agentvr.h"
#include "mvrcontroller.h"
#include "mwindow.h"


const string KCont_Text = "Text";

AAgentVr::AAgentVr(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Text);
}

AAgentVr::~AAgentVr()
{
}

string AAgentVr::PEType()
{
    return AVWidget::PEType() + GUri::KParentSep + Type();
}

MIface* AAgentVr::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVrp::Type()) == 0) {
	res = dynamic_cast<MVrp*>(this);
    } else {
	res = AVWidget::DoGetObj(aName);
    }
    return res;
}

TBool AAgentVr::HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName)
{
    TBool res = EFalse;
    res = AVWidget::HandleCompChanged(aContext, aComp, aContName);
    return res;
}


void AAgentVr::Render()
{
    __ASSERT(mIsInitialised);

    float xc = (float) GetParInt("./AlcX");
    float yc = (float) GetParInt("./AlcY");
    float wc = (float) GetParInt("./AlcW");
    float hc = (float) GetParInt("./AlcH");

    if (iMan->Name() == "?") {
	Logger()->Write(EInfo, this, "Render");
    }
    // Get viewport parameters
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    int vp_width = viewport[2], vp_height = viewport[3];

    glColor3f(mBgColor.r, mBgColor.g, mBgColor.b);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)vp_width, 0, (GLdouble)vp_height, -1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(xc, yc);
    glVertex2f(xc, yc + hc);
    glVertex2f(xc + wc, yc + hc);
    glVertex2f(xc + wc, yc);
    glEnd();

    glFlush();
    CheckGlErrors();
}

void AAgentVr::DrawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}


const string KTitle = "Hellow World!";
const string KStateContVal = "Value";


AUnitCrp::AUnitCrp(const string& aName, MUnit* aMan, MEnv* aEnv): AAgentVr(aName, aMan, aEnv), mFont(NULL),
    mEnv(nullptr), mMdl(nullptr)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

AUnitCrp::~AUnitCrp()
{
    if (mFont) {
	delete mFont;
    }
}

string AUnitCrp::PEType()
{
    return AAgentVr::PEType() + GUri::KParentSep + Type();
}

MIface* AUnitCrp::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    res = AAgentVr::DoGetObj(aName);
    return res;
}

void AUnitCrp::Render()
{
    __ASSERT(mIsInitialised);

    float xc = (float) GetParInt("./AlcX");
    float yc = (float) GetParInt("./AlcY");
    float wc = (float) GetParInt("./AlcW");
    float hc = (float) GetParInt("./AlcH");

    if (iMan->Name() == "?") {
	Logger()->Write(EInfo, this, "Render");
    }
    // Get viewport parameters
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    int vp_width = viewport[2], vp_height = viewport[3];

    glColor3f(mBgColor.r, mBgColor.g, mBgColor.b);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)vp_width, 0, (GLdouble)vp_height, -1.0, 1.0);
    glLineWidth(K_LineWidth);

    // Window coordinates
    int wlx = 0, wty = 0, wrx = 0, wby = 0;
    getWndCoord(0, 0, wlx, wty);
    getWndCoord(wc, hc, wrx, wby);
    int wndWidth = 0, wndHeight = 0;
    Wnd()->GetFbSize(&wndWidth, &wndHeight);
    int wwty = wndHeight - wty;
    int wwby = wwty - hc;

    // Background
    glColor3f(mBgColor.r, mBgColor.g, mBgColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(wlx, wwty);
    glVertex2f(wlx, wwby);
    glVertex2f(wrx, wwty);
    glVertex2f(wrx, wwby);
    glEnd();

    // Draw border
    glColor3f(mFgColor.r, mFgColor.g, mFgColor.b);
    DrawLine(wlx, wwty, wlx, wwby);
    DrawLine(wlx, wwby, wrx, wwby);
    DrawLine(wrx, wwby, wrx, wwty);
    DrawLine(wrx, wwty, wlx, wwty);
    // Draw name divider
    int nameDivH = K_BFontSize + 2 * K_BPadding;
    int wys = wwty - nameDivH;
    DrawLine(wlx, wys, wrx, wys);
    // Draw the name
    const string& titleText = (mMdl != nullptr) ? mMdl->Name() : KTitle;
    glRasterPos2f(wlx + K_BPadding, wys + K_BPadding);
    mFont->Render(titleText.c_str());

    CheckGlErrors();
}

void AUnitCrp::Init()
{
    AAgentVr::Init();

    string fontPath = iMan->GetContent(KCnt_FontPath);
    mFont = new FTPixmapFont(fontPath.c_str());
    mFont->FaceSize(K_BFontSize);
    const string& titleText = (mMdl != nullptr) ? mMdl->Name() : KTitle;
    int adv = (int) mFont->Advance(titleText.c_str());
    int tfh = (int) mFont->LineHeight();
    MUnit* host = GetMan();
    MUnit* rw = host->GetNode("./RqsW");
    MUnit* rh = host->GetNode("./RqsH");
    // Requisition
    string data = "SI " + to_string(adv + 2 * K_BPadding);
    rw->ChangeCont(data, true, KStateContVal);
    int minRh = K_MinBodyHeight + tfh + 2 * K_BPadding;
    data = "SI " + to_string(minRh);
    rh->ChangeCont(data, true, KStateContVal);
}

string AUnitCrp::MVrp_Mid() const
{
    return GetUri(NULL, true);
}

void AUnitCrp::SetEnv(MEnv* aEnv)
{
    __ASSERT(mEnv == nullptr && aEnv != nullptr);
    mEnv = aEnv;
}

void AUnitCrp::SetModel(const string& aMdlUri)
{
    __ASSERT(mMdl == nullptr);
    MUnit* mdl = iEnv->Root()->GetNode(aMdlUri);
    __ASSERT(mdl != nullptr);
    mMdl = mdl;
}

bool AUnitCrp::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    bool res = false;
    if (aButton == EFvBtnLeft && aAction == EFvBtnActPress) {
	double x = 0, y = 0;
	GetCursorPosition(x, y);
	if (IsInnerWidgetPos(x, y)) {
	    MUnit* host = GetMan();
	    MUnit* owner = host->GetMan();
	    //cout << "UnitCrp [" << iMan->Name() << "], button" << endl;
	    MViewMgr* view = owner->GetSIfit(view, this);
	    if (view) {
		MVrpView* vrpView = dynamic_cast<MVrpView*>(view->DoGetIface(MVrpView::Type()));
		if (vrpView) {
		    vrpView->OnCompSelected(this);
		    res = true;
		}
	    }
	}
    }
    return res;
}

string AUnitCrp::GetModelUri() const
{
    __ASSERT(mMdl);
    return mMdl->GetUri(NULL, true);
}
	

// Unit DRP

const string K_CpInpModelUri = "./InpModelUri";
const string K_CpOutModelUri = "./OutModelUri";

AUnitDrp::AUnitDrp(const string& aName, MUnit* aMan, MEnv* aEnv): AHLayoutL(aName, aMan, aEnv),
    mEnv(nullptr), mMdl(nullptr)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AUnitDrp::PEType()
{
    return AHLayoutL::PEType() + GUri::KParentSep + Type();
}

MIface* AUnitDrp::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVrp::Type()) == 0) {
	res = dynamic_cast<MVrp*>(this);
    } else {
	res = AHLayoutL::DoGetObj(aName);
    }
    return res;
}

void AUnitDrp::Render()
{
    AHLayoutL::Render();
}

string AUnitDrp::MVrp_Mid() const
{
    return GetUri(NULL, true);
}

void AUnitDrp::SetEnv(MEnv* aEnv)
{
    __ASSERT(mEnv == nullptr && aEnv != nullptr);
    mEnv = aEnv;
}

void AUnitDrp::SetModel(const string& aMdlUri)
{
    __ASSERT(mMdl == nullptr);
    MUnit* mdl = iEnv->Root()->GetNode(aMdlUri);
    __ASSERT(mdl != nullptr);
    mMdl = mdl;
    CreateRp();
}

void AUnitDrp::CreateRp()
{
    // Remove current slots first
    RmAllSlots();
    // Create new slots
    MUnit* host = GetMan();
    for (int ind = 0; ind < mMdl->CompsCount(); ind++) {
	MUnit* comp = mMdl->GetComp(ind);
	string compUri = comp->GetUri(0,true);
	InsertWidget(comp->Name(), "/*/Modules/FvWidgetsL/FUnitCrp", KPosEnd);
	MUnit* vcompu = host->GetNode("./" + comp->Name());
	__ASSERT(vcompu != nullptr);
	MVrp* vcompr = vcompu->GetSIfit(vcompr);
	__ASSERT(vcompr != nullptr);
	//vcompr->SetEnv(mEnv);
	vcompr->SetModel(compUri);
    }
}

void AUnitDrp::DestroyRp()
{
}

void AUnitDrp::SetCrtlBinding(const string& aCtrUri)
{
    __ASSERT(mCtrBnd.empty());
    MUnit* ctru = GetNode(aCtrUri);
    if (ctru) {
	mCtrBnd = aCtrUri;
    } else {
	Logger()->Write(EErr, this, "Wrong controller binding info [%s]", aCtrUri.c_str());
    }
}

void AUnitDrp::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    TIfRange rr;
    TICacheRCtx ctx(aCtx);
    if (aName == MVrController::Type()) {
	// Follow binding info
	if (!mCtrBnd.empty()) {
	    MUnit* ctru = GetNode(mCtrBnd);
	    if (ctru) {
		rr = ctru->GetIfi(aName, ctx);
		InsertIfCache(aName, aCtx, ctru, rr);
	    }
	}
    } else if (aName == MDesInpObserver::Type()) {
	MUnit* inpMdlUri = iMan->GetNode(K_CpInpModelUri);
	if (ctx.IsInContext(inpMdlUri)) {
	    MIface* iface = dynamic_cast<MDesInpObserver*>(&mIapModelUri);
	    InsertIfCache(aName, aCtx, inpMdlUri, iface);
	}
    } else if (aName == MDVarGet::Type()) {
	MUnit* outMdlUri = iMan->GetNode(K_CpOutModelUri);
	if (ctx.IsInContext(outMdlUri)) {
	    MIface* iface = dynamic_cast<MDVarGet*>(&mPapModelUri);
	    InsertIfCache(aName, aCtx, outMdlUri, iface);
	}
    } else {
	AHLayoutL::UpdateIfi(aName, aCtx);
    }
}

void AUnitDrp::GetModelUri(Sdata<string>& aData)
{
    aData.mData = GetModelUri();
    aData.mValid = true;
}

string AUnitDrp::GetModelUri() const
{
    return mMdl ? mMdl->GetUri(NULL, true) : GUri::Nil();
}

void AUnitDrp::OnInpModelUri()
{
    mModelUriChanged = ETrue;
    SetActive();
}

void AUnitDrp::ApplyModelUri()
{
    MUnit* inp = iMan->GetNode(K_CpInpModelUri);
    if (inp) {
	string uris;
	TBool res = GetSData(inp, uris);
	if (res) {
	    if (uris != mModelUri && uris != GUri::Nil()) {
		MUnit* mdl = iMan->GetNode(uris);
		if (mdl) {
		    mMdl = mdl;
		    CreateRp();
		    NotifyOnMdlUpdated();
		    Logger()->Write(EDbg, this, "Model applied [%s]", uris.c_str());
		} else {
		    Logger()->Write(EErr, this, "Couldn't find the model [%s]", uris.c_str());
		}
		mModelUri = uris;
	    }
	}
    }
}

void AUnitDrp::NotifyOnMdlUpdated()
{
    MUnit* outMdlUri = iMan->GetNode(K_CpOutModelUri);
    if (outMdlUri) {
	TIfRange ifr = outMdlUri->GetIfi(MDesInpObserver::Type());
	for (auto it = ifr.first; it != ifr.second; it++) {
	    MDesInpObserver* mobs = dynamic_cast<MDesInpObserver*>(*it);
	    mobs->OnInpUpdated();
	}
    }
}

void AUnitDrp::Update()
{
    if (mModelUriChanged) {
	ApplyModelUri();
	mModelUriChanged = EFalse;
    }
    AHLayoutL::Update();
}

void AUnitDrp::SetActive()
{
    // We need to "activate" all pseudo-states
    //mModelUriChanged = true;
    AHLayoutL::SetActive();
}

void AUnitDrp::ForceActive()
{
    // We need to "activate" all pseudo-states
    mModelUriChanged = true;
    AHLayoutL::ForceActive();
}

// Agents Visual representation view manager


AVrpView::AVrpView(const string& aName, MUnit* aMan, MEnv* aEnv): Unit(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVrpView::PEType()
{
    return Unit::PEType() + GUri::KParentSep + Type();
}

MIface* AVrpView::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVrpView::Type()) == 0) {
	res = dynamic_cast<MVrpView*>(this);
    } else if (strcmp(aName, MViewMgr::Type()) == 0) {
	res = dynamic_cast<MViewMgr*>(this);
    } else if (strcmp(aName, MAgent::Type()) == 0) {
	res = dynamic_cast<MAgent*>(this);
    } else {
	res = Unit::DoGetObj(aName);
    }
    return res;
}

MIface* AVrpView::MAgent_DoGetIface(const string& aUid)
{
    MIface* res = NULL;
    if (aUid == MUnit::Type())
	res = dynamic_cast<MUnit*>(this);
    return res;
}

void AVrpView::OnCompSelected(const MVrp* aComp)
{
    string selUri = aComp->GetModelUri();
    MElem* eowner = iMan->GetObj(eowner);
    string newVal = "SS " + selUri;
    eowner->AppendMutation(TMut(ENt_Cont, ENa_Targ, "./NodeSelected", ENa_Id, "Value", ENa_MutVal, newVal));
    TNs ns; MutCtx mctx(NULL, ns);
    eowner->Mutate(true, false, false, mctx);
    Logger()->Write(EInfo, this, "NodeSelected, updated to [%s]", newVal.c_str());
}

MIface* AVrpView::MViewMgr_DoGetIface(const string& aName)
{
    MIface* res = NULL;
    if (aName == MVrpView::Type()) res = dynamic_cast<MVrpView*>(this);
    return res;
}
