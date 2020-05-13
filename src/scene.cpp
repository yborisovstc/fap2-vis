

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "mscel.h"
#include "mwindow.h"
#include "mcontainer.h"

const string KWndCnt_Init = "Init";
const string KWndCnt_Init_Val = "Yes";

AGtScene::AGtScene(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mWndInit(EFalse)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AGtScene::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AGtScene::Construct()
{
}

MIface *AGtScene::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MScene::Type()) == 0) {
	res = dynamic_cast<MScene*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

void AGtScene::RenderScene(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    MUnit* host = GetMan();
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* comp = host->GetComp(ind);
	MSceneElem* mse = (MSceneElem*) comp->GetSIfi(MSceneElem::Type());
	if (mse != NULL) {
	    mse->Render();
	}
    }
    glFlush();
}

void AGtScene::Update()
{
    ADes::Update();
}

void AGtScene::onCursorPosition(double aX, double aY)
{
    MUnit* host = GetMan();
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* comp = host->GetComp(ind);
	MSceneElem* mse = (MSceneElem*) comp->GetSIfi(MSceneElem::Type());
	if (mse != NULL) {
	    mse->onSeCursorPosition(aX, aY);
	}
    }
}

void AGtScene::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    MUnit* host = GetMan();
    for (int ind = 0; ind < host->CompsCount(); ind++) {
	MUnit* comp = host->GetComp(ind);
	MSceneElem* mse = (MSceneElem*) comp->GetSIfi(MSceneElem::Type());
	if (mse != NULL) {
	    mse->onMouseButton(aButton, aAction, aMods);
	}
    }
}

void AGtScene::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    TIfRange rr;
    TICacheRCtx ctx(aCtx);
    if (aName == MWindow::Type()) {
	// Redirect to owner
	rr = host->GetMan()->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, host->GetMan(), rr);
    } else if (aName == MViewMgr::Type() && !aCtx.empty() && iMan->IsComp(aCtx.front())) {
	    // For view manager redirect upward
	    host = iMan->GetMan();
	    rr = host->GetIfi(aName, ctx);
	    InsertIfCache(aName, aCtx, host, rr);
    } else {
	ADes::UpdateIfi(aName, aCtx);
    }
}
