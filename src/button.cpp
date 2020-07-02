
#include <iostream> 
#include <FTGL/ftgl.h>

#include "button.h"
#include "mwindow.h"

const string KCont_Text = "Text";
const string KStateContVal = "Value";

AButton::AButton(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Text);
}

AButton::~AButton()
{
}

MUnit* AButton::GetStatePressed()
{
    return iMan->GetNode("./Pressed");
}

string AButton::PEType()
{
    return AVWidget::PEType() + GUri::KParentSep + Type();
}

MIface* AButton::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    res = AVWidget::DoGetObj(aName);
    return res;
}

TBool AButton::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = EFalse;
    res = AVWidget::HandleCompChanged(aContext, aComp, aContName);
    return res;
}

bool AButton::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    bool res = false;
    if (aButton == GLFW_MOUSE_BUTTON_LEFT && aAction == GLFW_PRESS) {
	double x = 0, y = 0;
	GetCursorPosition(x, y);
	if (IsInnerWidgetPos(x, y)) {
	    MUnit* host = GetMan();
	    MUnit* owner = host->GetMan();
	    //cout << "UnitCrp [" << iMan->Name() << "], button" << endl;
	    MUnit* spressed = GetStatePressed();
	    spressed->ChangeCont("SB true", true, KStateContVal);
	}
    }
    return res;
}

void AButton::Render()
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
    // Draw the name
    const string btnText = iMan->GetContent(KCont_Text);
    glRasterPos2f(wlx + K_BPadding, wwby + K_BPadding);
    mFont->Render(btnText.c_str());

    CheckGlErrors();
}

void AButton::Init()
{
    AVWidget::Init();

    string fontPath = iMan->GetContent(KCnt_FontPath);
    mFont = new FTPixmapFont(fontPath.c_str());
    mFont->FaceSize(K_BFontSize);
    const string btnText = iMan->GetContent(KCont_Text);
    int adv = (int) mFont->Advance(btnText.c_str());
    int tfh = (int) mFont->LineHeight();
    float llx, lly, llz, urx, ury, urz;
    mFont->BBox(btnText.c_str(), llx, lly, llz, urx, ury, urz);
    MUnit* host = GetMan();
    MUnit* rw = host->GetNode("./RqsW");
    MUnit* rh = host->GetNode("./RqsH");
    // Requisition
    //string data = "SI " + to_string(adv + 2 * K_BPadding);
    int minRw = (int) urx + 2 * K_BPadding;
    string data = "SI " + to_string(minRw);
    rw->ChangeCont(data, true, KStateContVal);
    //int minRh = tfh + 2 * K_BPadding;
    int minRh = (int) ury + 2 * K_BPadding;
    data = "SI " + to_string(minRh);
    rh->ChangeCont(data, true, KStateContVal);
}
