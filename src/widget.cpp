
#include "widget.h"
#include <mdata.h>
#include <iostream>

using namespace std;

void HndlGdkEvent(GdkEvent *event, gpointer data)
{
    AVisEnv* self = (AVisEnv*) data;
    self->OnEvent(event);
}

AVisEnv::AVisEnv(const string& aName, MElem* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv)
{
    SetParent(Type());
    //Construct();
}

AVisEnv::AVisEnv(MElem* aMan, MEnv* aEnv): ADes(Type(), aMan, aEnv)
{
    SetParent(Elem::PEType());
    //Construct();
}

void AVisEnv::Construct()
{
    // Init GDK
    g_type_init ();
    gdk_init(0, NULL);
    gdk_rgb_init();
    gdk_event_handler_set(HndlGdkEvent, this, NULL);
}

AVisEnv::~AVisEnv()
{
}

void AVisEnv::OnEvent(GdkEvent* event)
{
    GdkEventType type = event->type;
    GdkEventAny any = event->any;
    cout << "Event: " << any.type << ", window: " << any.window << endl;

    for (TInt ci = 0; ci < CompsCount(); ci++) {
	MElem* eit = GetComp(ci);
	//MGdkEventHandler* mhandler = (MGdkEventHandler*) eit->GetSIfiC(MGdkEventHandler::Type(), this);
	// TODO To consider specific CP "GdkEventHandler" for Gwindow
	MGWidgetComp* mhandler = (MGWidgetComp*) eit->GetSIfiC(MGWidgetComp::Type(), this);
	if (mhandler != NULL) {
	    //mhandler->OnEvent(event);
	    mhandler->OnGdkEvent(event);
	}
    }


    if (type == GDK_EXPOSE) {
    } else if (type == GDK_WINDOW_STATE) {
	GdkEventWindowState evt = event->window_state;
    }
}

string AVisEnv::PEType()
{
    return AStateWnd::PEType() + GUri::KParentSep + Type();
}

void *AVisEnv::DoGetObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDesSyncable::Type()) == 0) {
	res = (MDesSyncable*) this;
    }
    else if (strcmp(aName, MDesObserver::Type()) == 0) {
	res = (MDesObserver*) this;
    }
    else {
	res = Elem::DoGetObj(aName);
    }
    return res;
}

void AVisEnv::UpdateIfi(const string& aName, const RqContext* aCtx)
{
    void* res = NULL;
    TIfRange rr;
    RqContext ctx(this, aCtx);
    if (strcmp(aName.c_str(), MDesObserver::Type()) == 0) {
	res = (MDesObserver*) this;
    } else if (strcmp(aName.c_str(), MDesSyncable::Type()) == 0) {
	res = (MDesSyncable*) this;
    } else {
	res = Elem::DoGetObj(aName.c_str());
    }
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    }
}

TBool AVisEnv::OnCompChanged(MElem& aComp, const string& aContName, TBool aModif)
{
    Construct();
    Elem::OnCompChanged(aComp, aContName);
}





// Gdk events handling mediator: just creates a "net" of MGdkEventHandler instances 

AGehMediator::AGehMediator(const string& aName, MElem* aMan, MEnv* aEnv): Elem(aName, aMan, aEnv)
{
    SetParent(Type());
}

AGehMediator::AGehMediator(MElem* aMan, MEnv* aEnv): Elem(Type(), aMan, aEnv)
{
    SetParent(Elem::PEType());
}

string AGehMediator::PEType()
{
    return Elem::PEType() + GUri::KParentSep + Type();
}

void *AGehMediator::DoGetObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MGdkEventHandler::Type()) == 0) {
	res = (MGdkEventHandler*) this;
    } else {
	res = Elem::DoGetObj(aName);
    }
    return res;
}

void AGehMediator::UpdateIfi(const string& aName, const RqContext* aCtx)
{
    void* res = NULL;
    TIfRange rr;
    RqContext ctx(this, aCtx);
    if (strcmp(aName.c_str(), MGdkEventHandler::Type()) == 0) {
	res = (MGdkEventHandler*) this;
    } else {
	res = Elem::DoGetObj(aName.c_str());
    }
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    }
}

void AGehMediator::OnEvent(GdkEvent* event)
{
    MElem* host = iMan->GetMan();
    if (host != NULL) {
	GdkEventType type = event->type;
	GdkEventAny any = event->any;
	//cout << "Mediator, Event: " << any.type << ", window: " << any.window << endl;
	for (TInt ci = 0; ci < host->CompsCount(); ci++) {
	    MElem* eit = host->GetComp(ci);
	    MGdkEventHandler* mhandler = (MGdkEventHandler*) eit->GetSIfiC(MGdkEventHandler::Type(), this);
	    if (mhandler != NULL) {
		mhandler->OnEvent(event);
	    }
	}
    }
}




// Base of Agent of widgets

// Data provider for GdkEventButton
string AGWidget::EventButtonProv::VarGetIfid()
{ 
    return MDtGet<NTuple>::Type();
}

void *AGWidget::EventButtonProv::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDtGet<NTuple>::Type()) == 0) res = (MDtGet<NTuple>*) this;
    return res;
}

void AGWidget::EventButtonProv::DtGet(NTuple& aData)
{
    mHost->GetBtnPressEvent(aData);
    aData.mValid = true;
}

// Data provider for ButoonRelease GdkEventButton
string AGWidget::EventButtonReleaseProv::VarGetIfid()
{ 
    return MDtGet<NTuple>::Type();
}

void *AGWidget::EventButtonReleaseProv::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDtGet<NTuple>::Type()) == 0) res = (MDtGet<NTuple>*) this;
    return res;
}

void AGWidget::EventButtonReleaseProv::DtGet(NTuple& aData)
{
    mHost->GetBtnReleaseEvent(aData);
    aData.mValid = true;
}


// Data provider for GdkEventMotion
string AGWidget::EventMotionProv::VarGetIfid()
{ 
    return MDtGet<NTuple>::Type();
}

void *AGWidget::EventMotionProv::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDtGet<NTuple>::Type()) == 0) res = (MDtGet<NTuple>*) this;
    return res;
}

void AGWidget::EventMotionProv::DtGet(NTuple& aData)
{
    mHost->GetMotionEvent(aData);
    aData.mValid = true;
}

// Data provider for Allocation
string AGWidget::AllocationProv::VarGetIfid()
{
    return MDtGet<NTuple>::Type();
}

void *AGWidget::AllocationProv::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDtGet<NTuple>::Type()) == 0) res = (MDtGet<NTuple>*) this;
    return res;
}

void AGWidget::AllocationProv::DtGet(NTuple& aData)
{
    aData = mHost->mAllocation;
    aData.mValid = true;
}

// Data provider for SizeRequest
string AGWidget::SizeRequestProv::VarGetIfid()
{
    return MDtGet<NTuple>::Type();
}

void *AGWidget::SizeRequestProv::DoGetDObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDtGet<NTuple>::Type()) == 0) res = (MDtGet<NTuple>*) this;
    return res;
}

void AGWidget::SizeRequestProv::DtGet(NTuple& aData)
{
    aData = mHost->mSizeRequest;
    aData.mValid = true;
}





const string KEventBtn_Srep = "TPL,SI:Type,SI:State,SI:Button,SI:X,SI:Y 0 0 0 0 0";
const string KMotionEvent_Srep = "TPL,SI:Type,SI:State,SI:X,SI:Y 0 0 0 0";
const string KAllocation_Srep = "TPL,SI:X,SI:Y,SI:W,SI:H 0 0 0 0";
const string KSizeRequest_Srep = "TPL,SI:W,SI:H 0 0 0 0";


string AGWidget::PEType()
{
    return Elem::PEType() + GUri::KParentSep + Type();
}

bool AGWidget::mInit = false;
AGWidget::tStatesMap  AGWidget::mStatesMap;

AGWidget::AGWidget(const string& aName, MElem* aMan, MEnv* aEnv): Elem(aName, aMan, aEnv),
    iX(0), iY(0), iH(10), iW(10), iActive(ETrue)
{
    SetParent(Type());
    Construct();
}

AGWidget::AGWidget(MElem* aMan, MEnv* aEnv): Elem(Type(), aMan, aEnv),
    iX(0), iY(0), iH(10), iW(10), iActive(ETrue)
{
    SetParent(Elem::PEType());
    Construct();
}

void AGWidget::Construct()
{
    mBtnPressEvtProv.SetHost(this);
    mBtnPressEvt.FromString(KEventBtn_Srep);
    mBtnReleaseEvtProv.SetHost(this);
    mBtnReleaseEvt.FromString(KEventBtn_Srep);
    mMotionEvtProv.SetHost(this);
    mMotionEvt.FromString(KMotionEvent_Srep);
    mAllocationProv.SetHost(this);
    mAllocation.FromString(KAllocation_Srep);
    mSizeRequestProv.SetHost(this);
    mSizeRequest.FromString(KSizeRequest_Srep);
    // TODO [YB] To use the published data "Widget_State" from module ../Widged_common
    // instead of the internal constants.
    if (!mInit) {
	mStatesMap[EWs_Normal] = string("Normal");
	mStatesMap[EWs_Active] = string("Active");
	mStatesMap[EWs_Prelight] = string("Prelight");
	mStatesMap[EWs_Selected] = string("Selected");
	mStatesMap[EWs_Insensitive] = string("Insensitive");
	mInit = true;
    }
}

void *AGWidget::DoGetObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, Type()) == 0) {
	res = this;
    } else if (strcmp(aName, MGWidget::Type()) == 0) {
	res = (MGWidget*) this;
    } else if (strcmp(aName, MGWidgetComp::Type()) == 0) {
	res = (MGWidgetComp*) this;
    } else if (strcmp(aName, MGWidgetOwner::Type()) == 0) {
	res = (MGWidgetOwner*) this;
    } else if (strcmp(aName, MACompsObserver::Type()) == 0) {
	res = (MACompsObserver*) this;
    } else if (strcmp(aName, MDesObserver::Type()) == 0) {
	res = (MDesObserver*) this;
    } 
    else {
	res = Elem::DoGetObj(aName);
    }
    return res;
}

void AGWidget::UpdateIfi(const string& aName, const RqContext* aCtx)
{
    void* res = NULL;
    TIfRange rr;
    TBool resg = EFalse;
    RqContext ctx(this, aCtx);
    if (strcmp(aName.c_str(), Type()) == 0) {
	res = this;
    } else if (strcmp(aName.c_str(), MDesObserver::Type()) == 0) {
	res = (MDesObserver*) this;
    } else if (strcmp(aName.c_str(), MDesSyncable::Type()) == 0) {
	res = (MDesSyncable*) this;
    } else if (strcmp(aName.c_str(), MGdkEventHandler::Type()) == 0) {
	res = (MGdkEventHandler*) this;

    } else if (strcmp(aName.c_str(), MDVarGet::Type()) == 0 || strcmp(aName.c_str(), MDtGet<Sdata<int> >::Type()) == 0) {
	bool isdvar = strcmp(aName.c_str(), MDVarGet::Type()) == 0;
	MElem* bpe = GetNode("./../../BtnPressEvent");
	MElem* bre = GetNode("./../../BtnReleaseEvent");
	MElem* mte = GetNode("./../../MotionEvent");
	MElem* alc = GetNode("./../../Allocation");
	if (aCtx->IsInContext(bpe)) {
	    if (isdvar) {
		res = (MDVarGet*) &mBtnPressEvtProv;
	    }
	} else if (aCtx->IsInContext(bre)) {
	    if (isdvar) {
		res = (MDVarGet*) &mBtnReleaseEvtProv;
	    }
	} else if (aCtx->IsInContext(mte)) {
	    if (isdvar) {
		res = (MDVarGet*) &mMotionEvtProv;
	    }
	} else if (aCtx->IsInContext(alc)) {
	    if (isdvar) {
		res = (MDVarGet*) &mAllocationProv;
	    }
	}
    } else {
	res = DoGetObj(aName.c_str());
    }
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    }
}

MGWidgetOwner* AGWidget::GetOwner()
{
    MGWidgetOwner* res = NULL;
    MElem* eprntcp = Host()->GetNode("./Comp");
    if (eprntcp != NULL) {
	res = (MGWidgetOwner*) eprntcp->GetSIfiC(MGWidgetOwner::Type(), this);
    } else {
	Logger()->Write(EErr, this, "Input [Comp] not exists");
    }
    return res;
}

GdkWindow* AGWidget::GetWindow()
{
    return NULL;
}

MElem* AGWidget::GetLayoutHint() const
{
    return NULL;
}

TBool AGWidget::HandleCompChanged(MElem& aContext, MElem& aComp, const string& aName)
{
    TBool res = ETrue;
    /*
    MElem* eprntcp = aContext.GetNode("./Comp");
    if (eprntcp != NULL) {
	if (eprntcp == &aComp || eprntcp->IsComp(&aComp)) {
	    if (iWnd == NULL) {
		MGWidgetOwner* mcont = (MGWidgetOwner*) eprntcp->GetSIfiC(MGWidgetOwner::Type(), this);
		if (mcont != NULL) {
		    GdkWindow* parent = mcont->GetWindow();
		    iWnd = gdk_window_new(parent, NULL);
		} else {
		    // TODO To destroy window?
		}
	    }
	}
    } else {
	Logger()->Write(EErr, this, "Input [Comp] not exists");
	res = EFalse;
    }
    */
    return res;
}

MElem* AGWidget::Host() 
{
    return iMan->GetMan();
}


bool AGWidget::GetDataInt(const string& aInpUri, int& aData) 
{
    bool res = false;
    MElem* einp = Host()->GetNode(aInpUri);
    if (einp != NULL) {
	MDVarGet* mvget = (MDVarGet*) einp->GetSIfiC(MDVarGet::Type(), this);
	if (mvget != NULL) {
	    MDtGet<Sdata<int> >* mdata = mvget->GetDObj(mdata);
	    if (mdata != NULL) {
		Sdata<int> sd;
		mdata->DtGet(sd);
		if (sd.mValid) {
		    aData = sd.mData;
		}
		res = true;
	    }
	}
    } else {
	Logger()->Write(EErr, this, "Input [%s] not exists", aInpUri.c_str());
    }
    return res;
}

bool AGWidget::GetInpState(const string& aInpUri, MGWidget::TState& aData)
{
    bool res = false;
    MElem* einp = Host()->GetNode(aInpUri);
    if (einp != NULL) {
	// Trying variable data
	MDVarGet* mvget = (MDVarGet*) einp->GetSIfiC(MDVarGet::Type(), this);
	if (mvget != NULL) {
	    MDtGet<Enum>* mdata = mvget->GetDObj(mdata);
	    if (mdata != NULL) {
		Enum sd;
		mdata->DtGet(sd);
		if (sd.mValid) {
		    MGWidget::TState data = (MGWidget::TState) sd.mData;
		    if (sd.mSet.at(data) == mStatesMap.at(data)) {
			aData = data;
		    }
		}
		res = true;
	    }
	}
    } else {
	Logger()->Write(EErr, this, "Input [%s] not exists", aInpUri.c_str());
    }
    return res;
}

void AGWidget::OnUpdated_X(int aOldData)
{
}

void AGWidget::OnUpdated_Y(int aOldData)
{
}

void AGWidget::OnUpdated_W(int aOldData)
{
}

void AGWidget::OnUpdated_H(int aOldData)
{
}

void AGWidget::OnUpdated_State(TState mOld)
{
}

void AGWidget::OnUpdated()
{
    SetActive();
}

void AGWidget::Update()
{
    // Data providers updated - update the data
    // X coord
    bool res;
    Logger()->Write(EInfo, this, "Update");
    int sX, oldX = iX;
    res = GetDataInt("./Inp_X/Int/PinData", sX);
    if (res && sX != iX) {
	iX = sX;
	// Set allocation data
	Sdata<int>* adata = dynamic_cast<Sdata<int>*> (mAllocation.GetElem("X"));
	__ASSERT(adata != NULL);
	adata->Set(iX);
	OnUpdated_X(oldX);
    }
    // Y coord
    int sY, oldY = iY;
    res = GetDataInt("./Inp_Y/Int/PinData", sY);
    if (res && sY != iY) {
	iY = sY;
	// Set allocation data
	Sdata<int>* adata = dynamic_cast<Sdata<int>*> (mAllocation.GetElem("Y"));
	__ASSERT(adata != NULL);
	adata->Set(iY);
	OnUpdated_Y(oldY);
    }
    // Width
    int sW, oldW = iW;
    res = GetDataInt("./Inp_W/Int/PinData", sW);
    if (res && sW != iW) {
	iW = sW;
	// Set allocation data
	Sdata<int>* adata = dynamic_cast<Sdata<int>*> (mAllocation.GetElem("W"));
	__ASSERT(adata != NULL);
	adata->Set(iW);
	OnUpdated_W(oldW);
    }
    // Heigth
    int sH, oldH = iH;
    res = GetDataInt("./Inp_H/Int/PinData", sH);
    if (res && sH != iH) {
	iH = sH;
	// Set allocation data
	Sdata<int>* adata = dynamic_cast<Sdata<int>*> (mAllocation.GetElem("H"));
	__ASSERT(adata != NULL);
	adata->Set(iH);
	OnUpdated_H(oldH);
    }
    // State
    TState state;
    res = GetInpState("./Inp_State/Int/PinData", state);
    if (res && state != mState) {
	TState oldState = mState;
	mState = state;
	OnUpdated_State(oldState);
    }
}

void AGWidget::Confirm()
{
}

TBool AGWidget::IsActive()
{
    return iActive;
}

void AGWidget::SetActive()
{
    iActive = ETrue;
}

void AGWidget::ResetActive()
{
    iActive = EFalse;
}

TBool AGWidget::IsUpdated()
{
    return iUpdated;
}

void AGWidget::SetUpdated()
{
    iUpdated = ETrue;
}

void AGWidget::ResetUpdated()
{
    iUpdated = EFalse;
}

void AGWidget::OnActivated()
{
}

bool AGWidget::OnButtonPress(GdkEventButton* aEvent)
{
    return HandleButtonPress(aEvent);
}

bool AGWidget::OnButtonRelease(GdkEventButton* aEvent)
{
    return HandleButtonRelease(aEvent);
}

bool AGWidget::OnMotion(GdkEventMotion* aEvent)
{
    return HandleMotion(aEvent);
}

bool AGWidget::HandleButtonPress(GdkEventButton* aEvent)
{
    // Set event data
    Sdata<int>* etype = dynamic_cast<Sdata<int>*> (mBtnPressEvt.GetElem("Type"));
    __ASSERT(etype != NULL);
    etype->Set(aEvent->type);
    Sdata<int>* estate = dynamic_cast<Sdata<int>*> (mBtnPressEvt.GetElem("State"));
    __ASSERT(estate != NULL);
    estate->Set(aEvent->state);
    Sdata<int>* ex = dynamic_cast<Sdata<int>*> (mBtnPressEvt.GetElem("X"));
    __ASSERT(ex != NULL);
    ex->Set(aEvent->x);
    Sdata<int>* ey = dynamic_cast<Sdata<int>*> (mBtnPressEvt.GetElem("Y"));
    __ASSERT(ey != NULL);
    ey->Set(aEvent->y);
    // Activate dependencies
    ActivateDeps("./../../BtnPressEvent/Int/PinObs");
    return false;
}

bool AGWidget::HandleButtonRelease(GdkEventButton* aEvent)
{
    // Set event data
    Sdata<int>* etype = dynamic_cast<Sdata<int>*> (mBtnReleaseEvt.GetElem("Type"));
    __ASSERT(etype != NULL);
    etype->Set(aEvent->type);
    Sdata<int>* estate = dynamic_cast<Sdata<int>*> (mBtnReleaseEvt.GetElem("State"));
    __ASSERT(estate != NULL);
    estate->Set(aEvent->state);
    Sdata<int>* ex = dynamic_cast<Sdata<int>*> (mBtnReleaseEvt.GetElem("X"));
    __ASSERT(ex != NULL);
    ex->Set(aEvent->x);
    Sdata<int>* ey = dynamic_cast<Sdata<int>*> (mBtnReleaseEvt.GetElem("Y"));
    __ASSERT(ey != NULL);
    ey->Set(aEvent->y);
    // Activate dependencies
    ActivateDeps("./../../BtnReleaseEvent/Int/PinObs");
    return false;
}

void AGWidget::ActivateDeps(const string& aUri) {
    MElem* eobs = GetNode(aUri);
    __ASSERT(eobs != NULL);
    RqContext ctx(this);
    TIfRange range = eobs->GetIfi(MDesObserver::Type());
    for (TIfIter it = range.first; it != range.second; it++) {
	MDesObserver* mobs = (MDesObserver*) (*it);
	if (mobs != NULL) {
	    mobs->OnUpdated();
	}
    }
}

bool AGWidget::HandleMotion(GdkEventMotion* aEvent)
{
    // Set event data
    Sdata<int>* etype = dynamic_cast<Sdata<int>*> (mMotionEvt.GetElem("Type"));
    __ASSERT(etype != NULL);
    etype->Set(aEvent->type);
    Sdata<int>* estate = dynamic_cast<Sdata<int>*> (mMotionEvt.GetElem("State"));
    __ASSERT(estate != NULL);
    estate->Set(aEvent->state);
    Sdata<int>* ex = dynamic_cast<Sdata<int>*> (mMotionEvt.GetElem("X"));
    __ASSERT(ex != NULL);
    ex->Set(aEvent->x);
    Sdata<int>* ey = dynamic_cast<Sdata<int>*> (mMotionEvt.GetElem("Y"));
    __ASSERT(ey != NULL);
    ey->Set(aEvent->y);
    // Activate dependencies
    ActivateDeps("./../../MotionEvent/Int/PinObs");
    return false;
}

void AGWidget::OnGdkEvent(GdkEvent* event)
{
    MElem* cp = Host()->GetNode("./Owner");
    // Propagete the event to childs
    RqContext ctx(this);
    TIfRange range = cp->GetIfi(MGWidgetComp::Type(), &ctx);
    for (TIfIter it = range.first; it != range.second; it++) {
	MGWidgetComp* comp = (MGWidgetComp*) (*it);
	if (comp != NULL) {
	    comp->OnGdkEvent(event);
	}
    }

}


// Top window

const string KWndCnt_Init = "Init";
const string KWndCnt_Init_Val = "Yes";

AGWindow::AGWindow(const string& aName, MElem* aMan, MEnv* aEnv): AGWidget(aName, aMan, aEnv), mWindow(NULL), mWndInit(EFalse)
{
    SetParent(Type());
}

AGWindow::AGWindow(MElem* aMan, MEnv* aEnv): AGWidget(Type(), aMan, aEnv), mWindow(NULL), mWndInit(EFalse)
{
    SetParent(Elem::PEType());
}

string AGWindow::PEType()
{
    return AGWidget::PEType() + GUri::KParentSep + Type();
}

void AGWindow::Construct()
{
    GdkDisplayManager* dm = gdk_display_manager_get();
    const gchar* dname = gdk_get_display_arg_name();
    GdkDisplay* d1 = gdk_display_open(NULL);
    gdk_display_manager_set_default_display (dm, d1);

    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);
    GdkWindow* rootwnd = gdk_screen_get_root_window(screen);
    GdkWindowType rwtype = gdk_window_get_window_type(rootwnd);
    int rw_width = gdk_window_get_width(rootwnd);
    int rw_height = gdk_window_get_height(rootwnd);

    gdk_rgb_init();
    GdkWindowAttr attr;
    attr.window_type = GDK_WINDOW_TOPLEVEL;
    attr.x = 0;
    attr.y = 0;
    attr.width = 400;
    attr.height = 500;
    attr.colormap = gdk_rgb_get_cmap();
    attr.wclass = GDK_INPUT_OUTPUT;
    gint mask =  GDK_WA_X | GDK_WA_Y | GDK_WA_COLORMAP;


    mWindow = gdk_window_new(rootwnd, &attr, mask); 
    GdkWindowType wtype = gdk_window_get_window_type(mWindow);
    GdkColor color;
    //gint cwres = gdk_color_white(attr.colormap, &color);
    color.green = 65535;
    gboolean cares = gdk_colormap_alloc_color(attr.colormap, &color, 0, 1);
    //gdk_window_move(mWindow, 200, 200);
    //gdk_window_move_resize(mWindow, 200, 200, 400, 300);
    gint cwres = gdk_color_white(attr.colormap, &color);
    //gdk_window_set_background(rootwnd, &color);
    gdk_window_show(rootwnd);
    gdk_window_set_background(mWindow, &color);
    gboolean wiv = gdk_window_is_visible(mWindow);
    gboolean rwiv = gdk_window_is_visible(rootwnd);
    GdkGC* wnd1gc = gdk_gc_new(mWindow);
    //gdk_gc_set_colormap(wnd1gc, attr.colormap);
    GdkColor color_drw1;
    color_drw1.red = 65535;
    cares = gdk_colormap_alloc_color(attr.colormap, &color_drw1, 0, 1);
    gdk_gc_set_foreground(wnd1gc, &color);
    gdk_draw_line(mWindow, wnd1gc, 5, 5, 40, 60); 
    gdk_draw_point(mWindow, wnd1gc, 2, 2);
    gdk_draw_rectangle(mWindow, wnd1gc, TRUE, 5, 5, 40, 60);
    gdk_window_show(mWindow);
    gdk_window_flush(mWindow);
}

void AGWindow::OnUpdated_X(int aOldData)
{
}

void AGWindow::OnUpdated_Y(int aOldData)
{
}

void AGWindow::OnUpdated_W(int aOldData)
{
    if (mWindow != NULL) {
	gdk_window_resize(mWindow, iW, iH);
    }
}

void AGWindow::OnUpdated_H(int aOldData)
{
    if (mWindow != NULL) {
	gdk_window_resize(mWindow, iW, iH);
    }
}

void AGWindow::Update()
{
    Logger()->Write(EInfo, this, "Update");
    if (!mWndInit) {
	// Checking in content flag showing that the window is part of visial env but not 
	// just base agent. If so, initialise the agent
	// TODO [YB] To find more suitable solution
	MElem* host = iMan->GetMan();
	string sCntInit = host->GetContent(KWndCnt_Init);
	if (sCntInit == KWndCnt_Init_Val) {
	    Construct();
	    mWndInit = ETrue;
	}
    }
    AGWidget::Update();
}


void AGWindow::OnGdkEvent(GdkEvent* event)
{
    GdkEventType type = event->type;
    GdkEventAny any = event->any;
    GdkWindow* wnd = any.window;
    if (wnd == mWindow) {
	cout << "Handling event: " << any.type << ", window: " << any.window << endl;
	if (type == GDK_DELETE) {
	    gdk_window_destroy(mWindow);
	}
    } else {
	AGWidget::OnGdkEvent(event);
    }
}





AStateWnd::AStateWnd(const string& aName, MElem* aMan, MEnv* aEnv): Elem(aName, aMan, aEnv), iActive(ETrue), iWnd(NULL),
    mInit(EFalse)
{
    SetParent(Type());
    //Construct();
}

AStateWnd::AStateWnd(MElem* aMan, MEnv* aEnv): Elem(Type(), aMan, aEnv), iActive(ETrue), iWnd(NULL),
    mInit(EFalse)
{
    SetParent(Elem::PEType());
    //Construct();
}

void AStateWnd::Construct()
{
    /*
    GdkDisplayManager* dm = gdk_display_manager_get();
    const gchar* dname = gdk_get_display_arg_name();
    GdkDisplay* d1 = gdk_display_open(NULL);
    gdk_display_manager_set_default_display (dm, d1);

    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);
    GdkWindow* rootwnd = gdk_screen_get_root_window(screen);
    GdkWindowType rwtype = gdk_window_get_window_type(rootwnd);
    int rw_width = gdk_window_get_width(rootwnd);
    int rw_height = gdk_window_get_height(rootwnd);

    gdk_rgb_init();
    GdkWindowAttr attr;
    attr.window_type = GDK_WINDOW_TOPLEVEL;
    attr.x = 20;
    attr.y = 20;
    attr.width = 100;
    attr.height = 100;
    attr.colormap = gdk_rgb_get_cmap();
    attr.wclass = GDK_INPUT_OUTPUT;
    gint mask =  GDK_WA_X | GDK_WA_Y | GDK_WA_COLORMAP;


    iWnd = gdk_window_new(rootwnd, &attr, mask); 
    GdkWindowType wtype = gdk_window_get_window_type(iWnd);
    GdkColor color;
    color.red = 65535;
    gdk_window_set_background(rootwnd, &color);
    gdk_window_show(rootwnd);
    gdk_window_set_background(iWnd, &color);
    gdk_window_show(iWnd);
    gdk_window_flush(iWnd);
    gboolean wiv = gdk_window_is_visible(iWnd);
    gboolean rwiv = gdk_window_is_visible(rootwnd);
    */



    GdkDisplayManager* dm = gdk_display_manager_get();
    const gchar* dname = gdk_get_display_arg_name();
    GdkDisplay* d1 = gdk_display_open(NULL);
    gdk_display_manager_set_default_display (dm, d1);

    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);
    GdkWindow* rootwnd = gdk_screen_get_root_window(screen);
    GdkWindowType rwtype = gdk_window_get_window_type(rootwnd);
    int rw_width = gdk_window_get_width(rootwnd);
    int rw_height = gdk_window_get_height(rootwnd);

    GdkWindowAttr attr;
    attr.window_type = GDK_WINDOW_TOPLEVEL;
    attr.x = 0;
    attr.y = 0;
    attr.width = 400;
    attr.height = 500;
    attr.colormap = gdk_rgb_get_cmap();
    attr.wclass = GDK_INPUT_OUTPUT;
    gint mask =  GDK_WA_X | GDK_WA_Y | GDK_WA_COLORMAP;


    iWnd = gdk_window_new(rootwnd, &attr, mask); 
    GdkWindowType wtype = gdk_window_get_window_type(iWnd);
    GdkColor color;
    //gint cwres = gdk_color_white(attr.colormap, &color);
    color.green = 65535;
    gboolean cares = gdk_colormap_alloc_color(attr.colormap, &color, 0, 1);
    //gdk_window_move(iWnd, 200, 200);
    //gdk_window_move_resize(iWnd, 200, 200, 400, 300);
    gint cwres = gdk_color_white(attr.colormap, &color);
    //gdk_window_set_background(rootwnd, &color);
    gdk_window_show(rootwnd);
    gdk_window_set_background(iWnd, &color);
    gboolean wiv = gdk_window_is_visible(iWnd);
    gboolean rwiv = gdk_window_is_visible(rootwnd);
    GdkGC* wnd1gc = gdk_gc_new(iWnd);
    //gdk_gc_set_colormap(wnd1gc, attr.colormap);
    GdkColor color_drw1;
    color_drw1.red = 65535;
    cares = gdk_colormap_alloc_color(attr.colormap, &color_drw1, 0, 1);
    gdk_gc_set_foreground(wnd1gc, &color);
    gdk_draw_line(iWnd, wnd1gc, 5, 5, 40, 60); 
    gdk_draw_point(iWnd, wnd1gc, 2, 2);
    gdk_draw_rectangle(iWnd, wnd1gc, TRUE, 5, 5, 40, 60);
    gdk_window_show(iWnd);
    gdk_window_flush(iWnd);
}

AStateWnd::~AStateWnd()
{
    if (iWnd != NULL) {
	if (!gdk_window_is_destroyed(iWnd)) {
	    gdk_window_destroy(iWnd);
	}
	delete iWnd;
    }
}

string AStateWnd::PEType()
{
    return AStateWnd::PEType() + GUri::KParentSep + Type();
}

void *AStateWnd::DoGetObj(const char *aName)
{
    void* res = NULL;
    if (strcmp(aName, MDesSyncable::Type()) == 0) {
	res = (MDesSyncable*) this;
    } else if (strcmp(aName, MDesObserver::Type()) == 0) {
	res = (MDesObserver*) this;
    } else if (strcmp(aName, MGdkEventHandler::Type()) == 0) {
	res = (MGdkEventHandler*) this;
    } else {
	res = Elem::DoGetObj(aName);
    }
    return res;
}

void AStateWnd::UpdateIfi(const string& aName, const RqContext* aCtx)
{
    void* res = NULL;
    TIfRange rr;
    RqContext ctx(this, aCtx);
    if (strcmp(aName.c_str(), MDesObserver::Type()) == 0) {
	res = (MDesObserver*) this;
    } else if (strcmp(aName.c_str(), MDesSyncable::Type()) == 0) {
	res = (MDesSyncable*) this;
    } else if (strcmp(aName.c_str(), MGdkEventHandler::Type()) == 0) {
	res = (MGdkEventHandler*) this;
    } else {
	res = Elem::DoGetObj(aName.c_str());
    }
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    }
}

TBool AStateWnd::IsActive()
{
    return iActive;
}

void AStateWnd::SetActive()
{
    iActive = ETrue;
}

void AStateWnd::ResetActive()
{
    iActive = EFalse;
}

void AStateWnd::Update()
{
    Logger()->Write(EInfo, this, "Update");
    if (!mInit) {
	Construct();
	mInit = ETrue;
    }
    //gdk_window_show(iWnd);

    // Width, height
    int cur_width = gdk_window_get_width(iWnd);
    int cur_height = gdk_window_get_height(iWnd);
    int sW, sH;
    bool res = GetDataInt("./Inp_W/Int/PinData", sW);
    res = res && GetDataInt("./Inp_H/Int/PinData", sH);
    if (res && (sW != cur_width || sH != cur_height)) {
	cout << "Update, size updated: " << sW << "," << sH << endl;
	gdk_window_resize(iWnd, sW, sH);
    }
    // Title
    string sTitle;
    res = GetDataString("./Inp_Title/Int/PinData", sTitle);
    if (sTitle != mTitle) {
	gdk_window_set_title(iWnd, sTitle.c_str());
	mTitle = sTitle;
    }
    ResetActive();
}

void AStateWnd::Confirm()
{
}

TBool AStateWnd::IsUpdated()
{
    return iUpdated;
}

void AStateWnd::SetUpdated()
{
    iUpdated = ETrue;
}

void AStateWnd::ResetUpdated()
{
    iUpdated = EFalse;
}

void AStateWnd::OnUpdated()
{
    SetActive();
}

void AStateWnd::OnActivated()
{
}

bool AStateWnd::GetDataInt(const string& aInpUri, int& aData) 
{
    bool res = false;
    MElem* einp = Host()->GetNode(aInpUri);
    if (einp != NULL) {
	MDVarGet* mvget = (MDVarGet*) einp->GetSIfiC(MDVarGet::Type(), this);
	if (mvget != NULL) {
	    MDtGet<Sdata<int> >* mdata = mvget->GetDObj(mdata);
	    if (mdata != NULL) {
		Sdata<int> sd;
		mdata->DtGet(sd);
		if (sd.mValid) {
		    aData = sd.mData;
		}
		res = true;
	    }
	}
    } else {
	Logger()->Write(EErr, this, "Input [%s] not exists", aInpUri.c_str());
    }
    return res;
}

bool AStateWnd::GetDataString(const string& aInpUri, string& aData) 
{
    bool res = false;
    MElem* einp = Host()->GetNode(aInpUri);
    if (einp != NULL) {
	MDVarGet* mvget = (MDVarGet*) einp->GetSIfiC(MDVarGet::Type(), this);
	if (mvget != NULL) {
	    MDtGet<Sdata<string> >* mdata = mvget->GetDObj(mdata);
	    if (mdata != NULL) {
		Sdata<string> sd;
		mdata->DtGet(sd);
		if (sd.mValid) {
		    aData = sd.mData;
		}
		res = true;
	    }
	}
    } else {
	Logger()->Write(EErr, this, "Input [%s] not exists", aInpUri.c_str());
    }
    return res;
}

MElem* AStateWnd::Host() 
{
    return iMan->GetMan();
}

void AStateWnd::OnEvent(GdkEvent* event)
{
    GdkEventType type = event->type;
    GdkEventAny any = event->any;
    GdkWindow* wnd = any.window;
    if (wnd == iWnd) {
	cout << "Handling event: " << any.type << ", window: " << any.window << endl;
	if (type == GDK_DELETE) {
	    gdk_window_destroy(iWnd);
	}
    }
}
