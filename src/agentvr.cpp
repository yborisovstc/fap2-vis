
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
    // Draw border
    DrawLine(xc, yc, xc, yc + hc);
    DrawLine(xc, yc + hc, xc + wc, yc + hc);
    DrawLine(xc + wc, yc + hc, xc + wc, yc);
    DrawLine(xc + wc, yc, xc, yc);
    // Draw name divider
    int nameDivH = K_BFontSize + 2 * K_BPadding;
    DrawLine(xc, yc + hc - nameDivH, xc + wc, yc + hc - nameDivH);
    // Draw the name
    //FTGLPixmapFont font(KFont);
    glRasterPos2f(xc + K_BPadding, yc + hc - nameDivH + K_BPadding);
    mFont->Render(KTitle.c_str());

    //glFlush();
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
