

#include <iostream> 
#include <rdata.h>

#include "widget.h"
#include "mwindow.h"

#include "deps/linmath.h" // Ref https://github.com/glfw/glfw/tree/master/deps

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

const string AVWidget::KCnt_BgColor = "BgColor";
const string AVWidget::KCnt_FgColor = "FgColor";
const string AVWidget::KCnt_FontPath = "FontPath";
const string KCnt_Color_R = "R";
const string KCnt_Color_G = "G";
const string KCnt_Color_B = "B";
const string KCnt_Color_A = "A";

static GLuint vertex_buffer, vertex_shader, fragment_shader;
static GLint mMvpLocation, vpos_location, vcol_location;

AVWidget::AVWidget(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVWidget::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

MIface* AVWidget::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MSceneElem::Type()) == 0) {
	res = dynamic_cast<MSceneElem*>(this);
    } else if (strcmp(aName, MACompsObserver::Type()) == 0) {
	res = dynamic_cast<MACompsObserver*>(this);
    } else if (strcmp(aName, MVCcomp::Type()) == 0) {
	res = dynamic_cast<MVCcomp*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

void AVWidget::Update()
{
    Logger()->Write(EInfo, this, "Update");
    ADes::Update();
}

void AVWidget::Confirm()
{
    Logger()->Write(EInfo, this, "Confirm");
    ADes::Confirm();
    if (!mIsInitialised) {
	Init();
	mIsInitialised = true;
    }
}

void AVWidget::CheckGlErrors()
{
    // check for errors
    const GLenum errCode = glGetError();
    if (errCode != GL_NO_ERROR){
	const GLubyte *errString;
	errString=gluErrorString(errCode);
	printf("error: %s\n", errString);
    }
}

int AVWidget::GetParInt(const string& aUri)
{
    int res = 0;
    MUnit* host = GetMan();
    MUnit* pu = host->GetNode(aUri);
    MDVarGet* pvg = pu->GetObj(pvg);
    MDtGet<Sdata<int>>* psi = pvg->GetDObj(psi);
    Sdata<int> pi = 0;
    psi->DtGet(pi);
    return pi.mData;
}

void AVWidget::Render()
{
    int xi = GetParInt("./AlcX");
    int yi = GetParInt("./AlcY");
    int wi = GetParInt("./AlcW");
    int hi = GetParInt("./AlcH");

    if (iMan->Name() == "Wdg1") {
	Logger()->Write(EInfo, this, "Render");
    }
    // Get viewport parameters
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    int vp_width = viewport[2], vp_height = viewport[3];

    float xc = (float) xi;
    float yc = (float) yi;
    float wc = (float) wi;
    float hc = (float) hi;

    //glClearColor(0.0, 0.0, 0.0, 0.0); // Don't clear window
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(mBgColor.r, mBgColor.g, mBgColor.b);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)vp_width, 0, (GLdouble)vp_height, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(xc, yc);
    glVertex2f(xc, yc + hc);
    glVertex2f(xc + wc, yc + hc);
    glVertex2f(xc + wc, yc);
    glEnd();

    glFlush();
    CheckGlErrors();
}

void AVWidget::Init()
{
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    CheckGlErrors();

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertex_shader);
    glAttachShader(mProgram, fragment_shader);
    glLinkProgram(mProgram);
    CheckGlErrors();

    mMvpLocation = glGetUniformLocation(mProgram, "MVP");
    vpos_location = glGetAttribLocation(mProgram, "vPos");
    vcol_location = glGetAttribLocation(mProgram, "vCol");
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
	    sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
	    sizeof(vertices[0]), (void*) (sizeof(float) * 2));
}

TBool AVWidget::HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName)
{
    TBool res = ETrue;

    if (aComp == GetMan()) {
	string val = aComp->GetContent(aContName);
	if (aContName == ContentCompId(KCnt_BgColor, KCnt_Color_R)) {
	    mBgColor.r = stof(val);
	} else if (aContName == ContentCompId(KCnt_BgColor, KCnt_Color_G)) {
	    mBgColor.g = stof(val);
	} else if (aContName == ContentCompId(KCnt_BgColor, KCnt_Color_B)) {
	    mBgColor.b = stof(val);
	} else if (aContName == ContentCompId(KCnt_FgColor, KCnt_Color_R)) {
	    mFgColor.r = stof(val);
	} else if (aContName == ContentCompId(KCnt_FgColor, KCnt_Color_G)) {
	    mFgColor.g = stof(val);
	} else if (aContName == ContentCompId(KCnt_FgColor, KCnt_Color_B)) {
	    mFgColor.b = stof(val);
	}
    }
    return res;
}

void AVWidget::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    TIfRange rr;
    TICacheRCtx ctx(aCtx);
    if (aName == MWindow::Type()) {
	// Redirect to owner
	rr = host->GetMan()->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, host->GetMan(), rr);
    } else {
	ADes::UpdateIfi(aName, aCtx);
    }
}

void AVWidget::onSeCursorPosition(double aX, double aY)
{
    // Window coordinates
    int wc = GetParInt("./AlcW");
    int hc = GetParInt("./AlcH");
    int wx0 = 0, wy0 = 0, wxw = 0, wyh = 0;
    getWndCoord(0, 0, wx0, wy0);
    getWndCoord(wc, hc, wxw, wyh);
    int wdX = aX - wx0;
    int wdY = aY - wy0;
    if (wdX >= 0 && wdX < wc && wdY >= 0 && wdY < hc) {
	onWdgCursorPos(wdX, wdY);
    }
}

bool AVWidget::IsInnerWidgetPos(double aX, double aY)
{
    int wc = GetParInt("./AlcW");
    int hc = GetParInt("./AlcH");
    int wlx = 0, wty = 0, wrx = 0, wby = 0;
    getWndCoord(0, 0, wlx, wty);
    getWndCoord(wc, hc, wrx, wby);
    int wndWidth = 0, wndHeight = 0;
    Wnd()->GetFbSize(&wndWidth, &wndHeight);
    int wwty = wndHeight - wty;
    int wwby = wwty - hc;
    return (aX > wlx && aX < wrx && aY > wwby && aY < wwty);
}

void AVWidget::onWdgCursorPos(int aX, int aY)
{
    //cout << "Widget [" << iMan->Name() << "], cursor, X: " << aX << ", Y:" << aY << endl;
}

void AVWidget::getWndCoord(int aInpX, int aInpY, int& aOutX, int& aOutY)
{
    MUnit* host = GetMan();
    MSceneElem* owner = (MSceneElem*) host->GetMan()->GetSIfi(MSceneElem::Type());
    if (owner) {
	int x = GetParInt("./AlcX");
	int y = GetParInt("./AlcY");
	owner->getWndCoord(x + aInpX, y + aInpY, aOutX, aOutY);
    } else {
	aOutX = aInpX;
	aOutY = aInpY;
    }
}

int AVWidget::WndX(int aX)
{
    int wx = 0, wy = 0;
    getWndCoord(aX, 0, wx, wy);
    return wx;
}

int AVWidget::WndY(int aY)
{
    int wx = 0, wy = 0;
    getWndCoord(0, aY, wx, wy);
    return wy;
}

bool AVWidget::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
    bool res = false;
    double x = 0, y = 0;
    GetCursorPosition(x, y);
    if (IsInnerWidgetPos(x, y)) {
	//cout << "Widget [" << iMan->Name() << "], button" << endl;
	res = true;
    }
    return res;
}

void AVWidget::GetCursorPosition(double& aX, double& aY)
{
    MWindow* wnd = Wnd();
    wnd->GetCursorPos(aX, aY);
}


MWindow* AVWidget::Wnd()
{
    MWindow* mwnd = (MWindow*) MUnit::GetSIfi(MWindow::Type());
    return mwnd;
}

void AVWidget::DrawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}
