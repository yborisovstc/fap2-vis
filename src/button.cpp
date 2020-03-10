
#include <iostream> 
#include <FTGL/ftgl.h>

#include "button.h"

const string KCont_Text = "Text";
const string KStateContVal = "Value";

const string KFont = "/home/yborisov/tmp/ubuntu-font-family-0.83/Ubuntu-B.ttf";

AButton::AButton(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Text);
}

AButton::~AButton()
{
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
	    cout << "UnitCrp [" << iMan->Name() << "], button" << endl;
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
    int wx0 = 0, wy0 = 0, wxw = 0, wyh = 0;
    getWndCoord(0, 0, wx0, wy0);
    getWndCoord(wc, hc, wxw, wyh);

    // Background
    glColor3f(mBgColor.r, mBgColor.g, mBgColor.b);
    glBegin(GL_POLYGON);
    glVertex2f(wx0, wy0);
    glVertex2f(wx0, wyh);
    glVertex2f(wxw, wyh);
    glVertex2f(wxw, wy0);
    glEnd();

    // Draw border
    glColor3f(mFgColor.r, mFgColor.g, mFgColor.b);
    DrawLine(wx0, wy0, wx0, wyh);
    DrawLine(wx0, wyh, wxw, wyh);
    DrawLine(wxw, wyh, wxw, wy0);
    DrawLine(wxw, wy0, wx0, wy0);
    // Draw the name
    //FTGLPixmapFont font(KFont);
    const string btnText = iMan->GetContent(KCont_Text);
    glRasterPos2f(WndX(K_BPadding), WndY(K_BPadding));
    mFont->Render(btnText.c_str());

    CheckGlErrors();
}

void AButton::Init()
{
    AVWidget::Init();

    mFont = new FTPixmapFont(KFont.c_str());
    mFont->FaceSize(K_BFontSize);
    const string btnText = iMan->GetContent(KCont_Text);
    int adv = (int) mFont->Advance(btnText.c_str());
    int tfh = (int) mFont->LineHeight();
    MUnit* host = GetMan();
    MUnit* rw = host->GetNode("./RqsW");
    MUnit* rh = host->GetNode("./RqsH");
    // Requisition
    string data = "SI " + to_string(adv + 2 * K_BPadding);
    rw->ChangeCont(data, true, KStateContVal);
    int minRh = tfh + 2 * K_BPadding;
    data = "SI " + to_string(minRh);
    rh->ChangeCont(data, true, KStateContVal);
}
