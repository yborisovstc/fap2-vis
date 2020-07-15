
#include <FTGL/ftgl.h>
#include "label.h"


const string KCont_Text = "Text";

AVLabel::AVLabel(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Text);
}

AVLabel::~AVLabel()
{
}

string AVLabel::PEType()
{
    return AVWidget::PEType() + GUri::KParentSep + Type();
}

MIface* AVLabel::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    res = AVWidget::DoGetObj(aName);
    return res;
}

TBool AVLabel::HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName)
{
    TBool res = false;

    if (aComp == GetMan() && aContName == KCont_Text) {
	res = ETrue;
    } 
    if (!res) {
	res = AVWidget::HandleCompChanged(aContext, aComp, aContName);
    }
    return res;
}


void AVLabel::Render()
{
    float xc = (float) GetParInt("./AlcX");
    float yc = (float) GetParInt("./AlcY");
    float wc = (float) GetParInt("./AlcW");
    float hc = (float) GetParInt("./AlcH");

    /* Create a pixmap font from a TrueType file. */
    string fontPath = iMan->GetContent(KCnt_FontPath);
    FTGLPixmapFont font(fontPath.c_str());

    if(font.Error()) {
	return;
    }

    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    int vp_width = viewport[2], vp_height = viewport[3];

    glClearColor(0.0, 0.0, 0.0, 0.0); // Don't clear window
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(mBgColor.r, mBgColor.g, mBgColor.b);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)vp_width, 0, (GLdouble)vp_height, -1.0, 1.0);
 
    // Set the font size and render a small text.
    font.FaceSize(hc);
    glRasterPos2f(xc, yc);
    font.Render("Hello World!");
    CheckGlErrors();
}
