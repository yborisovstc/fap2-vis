
#include <iostream>

#include "visenv.h"
#include <mdata.h>
#include <iostream>
#include <functional>
#include "mdata.h"
#include "mscene.h"

#include "GL/glew.h"
#include "GL/gl.h"
#include <GLFW/glfw3.h>

using namespace std;

const string AVisEnv::mCont_Init = "Init";


AVisEnv::AVisEnv(const string& aName, MUnit* aMan, MEnv* aEnv): Unit(aName, aMan, aEnv)
{
    // For native agt (aName is empty) set type as name. For inherited agent set given name
    iName = aName.empty() ? GetType(PEType()) : aName;
    // Don't construct native agent here. Only heirs needs to be constructed fully.
    //Construct();
}

void AVisEnv::Construct()
{
    if (!glfwInit()) {
	// TODO handle error
    }
}

AVisEnv::~AVisEnv()
{
    glfwTerminate();
}

string AVisEnv::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

MIface *AVisEnv::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVisEnv::Type()) == 0) {
	res = dynamic_cast<MVisEnv*>(this);
    } else {
	res = Unit::DoGetObj(aName);
    }
    return res;
}

TBool AVisEnv::OnCompChanged(MUnit& aComp, const string& aContName, TBool aModif)
{
    if (aContName == mCont_Init) {
	Construct();
    }

    Unit::OnCompChanged(aComp, aContName);
}


// Top window

const string KWndCnt_Init = "Init";
const string KWndCnt_Init_Val = "Yes";

vector<AGWindow*> AGWindow::mInstances = {}; //!< Register of instances

AGWindow::AGWindow(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mWndInit(EFalse),
    mWindow(NULL)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    if (!mWndInit) {
	// Checking in content flag showing that the window is part of visial env but not 
	// just base agent. If so, initialise the agent
	// TODO [YB] To find more suitable solution
	Construct();
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetWindowSizeCallback(mWindow, onWindowSizeChanged);
	mWndInit = ETrue;
    }
}

string AGWindow::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AGWindow::Construct()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /*
    MUnit* host = GetMan();

    MUnit* wu = host->GetNode("./Width");
    MDVarGet* wvg = wu->GetObj(wvg);
    MDtGet<Sdata<int>>* wsi = wvg->GetDObj(wsi);
    Sdata<int> wi = 0;
    wsi->DtGet(wi);

    MUnit* hu = host->GetNode("./Heigth");
    MDVarGet* hvg = hu->GetObj(wvg);
    MDtGet<Sdata<int>>* hsi = hvg->GetDObj(wsi);
    Sdata<int> hi = 0;
    hsi->DtGet(hi);
    */

    Sdata<int> wi = 640;
    Sdata<int> hi = 480;
    mWindow = glfwCreateWindow(wi.mData, hi.mData, "My Title", NULL, NULL);
    if (mWindow != NULL) {
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetWindowCloseCallback(mWindow, onWindowClosed);
	glfwSetCursorPosCallback(mWindow, onCursorPosition);
	glfwSetMouseButtonCallback(mWindow, onMouseButton);
	glfwMakeContextCurrent(mWindow);
	//gladLoadGL(glfwGetProcAddress);
	// TODO  YB!! This interval affects window refreshing. With value 1 the unitvr frame is rendered only partially
	// To investigate
	glfwSwapInterval(2); //YB!!
	glewInit();
	// Register the window instance
	RegisterInstance(this);
	// Set viewport
	glViewport(0, 0, wi.mData, hi.mData);
    } else {
	// Window or context creation failed
    }
}

void AGWindow::RegisterInstance(AGWindow* aInst)
{
    mInstances.push_back(aInst);
}

AGWindow* AGWindow::FindInstance(GLFWwindow* aWnd)
{
    AGWindow* res = NULL;
    for (auto inst: mInstances) {
	if (inst->RawWindow() == aWnd) {
	    res = inst;
	    break;
	}
    }
    return res;
}

MDVarSet* AGWindow::StWidth()
{
    MDVarSet* res = NULL;
    MUnit* widthu = Host() == NULL ? NULL: Host()->GetNode("./Width");
    if (widthu != NULL) {
	res = widthu->GetObj(res);
    }
    return res;
}

void AGWindow::onWindowSizeChanged (GLFWwindow *aWnd, int aW, int aH)
{
    AGWindow* wnd = FindInstance(aWnd);
    if (wnd != NULL) {
	MDVarSet* widthdv = wnd->StWidth();
	if (widthdv != NULL) {
	    MDtSet<Sdata<TInt>>* width = dynamic_cast<MDtSet<Sdata<TInt>>*>(widthdv->DoGetSDObj(MDtSet<Sdata<TInt>>::Type()));
	    if (width != NULL) {
		width->DtSet(aW);
	    }
	}
    }
}

void AGWindow::onWindowClosed(GLFWwindow *aWnd)
{
    AGWindow* wnd = reinterpret_cast<AGWindow*>(glfwGetWindowUserPointer(aWnd));
    if (wnd != NULL) {
	wnd->onWindowClosed();
    }
}

void AGWindow::onCursorPosition(GLFWwindow *aWnd, double aX, double aY)
{
    AGWindow* wnd = reinterpret_cast<AGWindow*>(glfwGetWindowUserPointer(aWnd));
    if (wnd != NULL) {
	wnd->onCursorPosition(aX, aY);
    }
}

void AGWindow::onMouseButton(GLFWwindow *aWnd, int aButton, int aAction, int aMods)
{
    AGWindow* wnd = reinterpret_cast<AGWindow*>(glfwGetWindowUserPointer(aWnd));
    if (wnd != NULL) {
	TFvButton btn = aButton == GLFW_MOUSE_BUTTON_RIGHT ? EFvBtnRight : EFvBtnLeft;
	TFvButtonAction act = aAction == GLFW_PRESS ? EFvBtnActPress : EFvBtnActRelease;
	wnd->onMouseButton(btn, act, aMods);
    }
}

void AGWindow::onWindowClosed()
{
    // Notify of closing
    OnError(this);
}

void AGWindow::onCursorPosition(double aX, double aY)
{
    cout << "Cursor, X: " << aX << ", Y: " << aY << endl;

    MUnit* scene = GetNode("./../Scene");
    if (scene != NULL) {
	MScene* mscene = (MScene*) scene->GetSIfi(MScene::Type(), this);
	if (mscene != NULL) {
	    int width, height;
	    glfwGetWindowSize(mWindow, &width, &height);
	    
	    mscene->onCursorPosition(aX, height - aY);
	}
    } else {
	Logger()->Write(EErr, this, "[%s] Missing scene", GetUri().c_str());
    }
}

void AGWindow::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    cout << "Window, onMouseButton" << endl;

    MUnit* scene = GetNode("./../Scene");
    if (scene != NULL) {
	MScene* mscene = (MScene*) scene->GetSIfi(MScene::Type(), this);
	if (mscene != NULL) {
	    mscene->onMouseButton(aButton, aAction, aMods);
	}
    } else {
	Logger()->Write(EErr, this, "[%s] Missing scene", GetUri().c_str());
    }
}

MIface *AGWindow::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MWindow::Type()) == 0) {
	res = dynamic_cast<MWindow*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}
/*
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
*/

void AGWindow::Render()
{
    Logger()->Write(EInfo, this, "Render");
    MUnit* scene = GetNode("./../Scene");
    if (scene != NULL) {
	MScene* mscene = (MScene*) scene->GetSIfi(MScene::Type(), this);
	if (mscene != NULL) {
	    mscene->RenderScene();
	}
    } else {
	Logger()->Write(EErr, this, "[%s] Missing scene", GetUri().c_str());
    }
}

void AGWindow::Update()
{
    Logger()->Write(EInfo, this, "Update");
    ADes::Update();
}

void AGWindow::Confirm()
{
    Logger()->Write(EInfo, this, "Confirm");
    ADes::Confirm();
    Render();
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

TBool AGWindow::OnCompChanged(MUnit& aComp, const string& aContName, TBool aModif)
{
    if (aContName == KWndCnt_Init) {
	//Construct();
    }
    Unit::OnCompChanged(aComp, aContName);
}

void AGWindow::GetCursorPos(double& aX, double& aY)
{
    double y = 0;
    glfwGetCursorPos(mWindow, &aX, &y);
    int width, height;
    glfwGetWindowSize(mWindow, &width, &height);
    aY = height - y;
}

void AGWindow::GetFbSize(int* aW, int* aH) const
{
    if (mWindow) {
	glfwGetWindowSize(mWindow, aW, aH);
    } else {
	*aW = -1; *aH = -1;
    }
}






string AVDesLauncher::PEType()
{
    return ADesLauncher::PEType() + GUri::KParentSep + Type();
}

AVDesLauncher::AVDesLauncher(const string& aName, MUnit* aMan, MEnv* aEnv): ADesLauncher(aName, aMan, aEnv)
{
    // TODO agent creation mechanism doesn't work if des.h:ADesLauncher is derived from Unit. Root cause
    // is that Unit based agents instances don't provide their type correctly because type getter isn't virtual
    // As for Elem it simulates virtual getter by storing type in instance specific data (chromo)
    SetCrAttr(PEType(), aName);
}

void AVDesLauncher::OnIdle()
{
    glfwPollEvents();
}

