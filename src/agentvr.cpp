
#include "agentvr.h"

#include <FTGL/ftgl.h>

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
    res = AVWidget::DoGetObj(aName);
    return res;
}

TBool AAgentVr::HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName)
{
    TBool res = EFalse;
    res = AVWidget::HandleCompChanged(aContext, aComp, aContName);
    return res;
}


void AAgentVr::Render()
{
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


const string KFont = "/home/yborisov/tmp/ubuntu-font-family-0.83/Ubuntu-B.ttf";
const string KTitle = "Hellow World!";
const string KStateContVal = "Value";


AUnitCrp::AUnitCrp(const string& aName, MUnit* aMan, MEnv* aEnv): AAgentVr(aName, aMan, aEnv), mFont(NULL)
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
    // Draw name divider
    int nameDivH = K_BFontSize + 2 * K_BPadding;
    int wys = WndY(hc - nameDivH);
    DrawLine(wx0, wys, wxw, wys);
    // Draw the name
    //FTGLPixmapFont font(KFont);
    glRasterPos2f(WndX(K_BPadding), WndY(hc - nameDivH + K_BPadding));
    mFont->Render(KTitle.c_str());

    CheckGlErrors();
}

void AUnitCrp::Init()
{
    AAgentVr::Init();

    mFont = new FTPixmapFont(KFont.c_str());
    mFont->FaceSize(K_BFontSize);
    int adv = (int) mFont->Advance(KTitle.c_str());
    MUnit* host = GetMan();
    MUnit* rw = host->GetNode("./RqsW");
    /*
    MDVarSet* rwvs = rw->GetObj(rwvs);
    MDtSet<Sdata<int> >* rwd = (MDtSet<Sdata<int> >*) rwvs->DoGetSDObj(MDtSet<Sdata<int> >::Type());
    if (rwd != NULL) {
	Sdata<int> arg;
	rwd->DtSet(arg);
	arg.mData = (int) adv;
    }
    */
    string data = "SI " + to_string(adv + 2 * K_BPadding);
    rw->ChangeCont(data, true, KStateContVal);
    rw->DumpContent();
}


// Unit DRP

AUnitDrp::AUnitDrp(const string& aName, MUnit* aMan, MEnv* aEnv): AVHLayout(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AUnitDrp::PEType()
{
    return AVHLayout::PEType() + GUri::KParentSep + Type();
}

MIface* AUnitDrp::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    res = AVHLayout::DoGetObj(aName);
    return res;
}

void AUnitDrp::Render()
{
    AVHLayout::Render();
}
