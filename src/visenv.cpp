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


static AVisEnv* sEnv = NULL;

AVisEnv::AVisEnv(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mIdleHandler(NULL)
{
    // For native agt (aName is empty) set type as name. For inherited agent set given name
    iName = aName.empty() ? GetType(PEType()) : aName;
    // Don't construct native agent here. Only heirs needs to be constructed fully.
    //Construct();
}

void renderFunction(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glEnd();
    glFlush();
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
    if (aName == MVisEnv::Type()) {
	res = dynamic_cast<MVisEnv*>(this);
    } else {
	res = ADes::DoGetObj(aName);
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

void AVisEnv::SetOnIdleHandler(TIdleHandler aHandler)
{
    __ASSERT(mIdleHandler == NULL);
    mIdleHandler = aHandler;
    //glutIdleFunc(mIdleHandler);
}

void AVisEnv::Display(void)
{
}

void Render()
{
    sEnv->Display();
}

void AVisEnv::Start(void)
{
    MUnit* wnd = GetNode("./../Window");
    if (wnd != NULL) {
	MWindow* mwnd = (MWindow*) wnd->GetSIfi(MWindow::Type(), this);
	if (mwnd != NULL) {
	    mwnd->Start();
	}
    }
}


// Top window

const string KWndCnt_Init = "Init";
const string KWndCnt_Init_Val = "Yes";

vector<AGWindow*> AGWindow::mInstances = {}; //!< Register of instances

AGWindow::AGWindow(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mWndInit(EFalse),
    mWindow(NULL)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AGWindow::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AGWindow::Construct()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    mWindow = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!mWindow)
    {
	// Window or context creation failed
    } else {
	glfwMakeContextCurrent(mWindow);
	//gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);
	glewInit();
	// Register the window instance
	RegisterInstance(this);
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
    if (!mWndInit) {
	// Checking in content flag showing that the window is part of visial env but not 
	// just base agent. If so, initialise the agent
	// TODO [YB] To find more suitable solution
	MUnit* host = iMan->GetMan();
	string sCntInit = host->GetContent(KWndCnt_Init);
	/*
	if (sCntInit == KWndCnt_Init_Val) {
	    Construct();
	    mWndInit = ETrue;
	}
	*/
	Construct();
	glfwSetWindowSizeCallback(mWindow, onWindowSizeChanged);
	mWndInit = ETrue;
	//glfwWaitEvents();
    } else {
	Render();
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
	//glfwWaitEvents();
	//SetUpdated();
    }
}

void AGWindow::Confirm()
{
    SetActive();
}

TBool AGWindow::OnCompChanged(MUnit& aComp, const string& aContName, TBool aModif)
{
    if (aContName == KWndCnt_Init) {
	Construct();
    }

    Unit::OnCompChanged(aComp, aContName);
}

void AGWindow::Start(void)
{
    while (!glfwWindowShouldClose(mWindow)) {
	Render();
	glfwWaitEvents();
    }
}
