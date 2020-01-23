

#include <rdata.h>

#include "widget.h"

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

void AVWidget::Update()
{
    Logger()->Write(EInfo, this, "Update");
    if (!mIsInitialised) {
	Init();
	mIsInitialised = true;
    } else {
	ADes::Update();
    }
}

static void CheckGlErrors()
{
    // check for errors
    const GLenum errCode = glGetError();
    if (errCode != GL_NO_ERROR){
	const GLubyte *errString;
	errString=gluErrorString(errCode);
	printf("error: %s\n", errString);
    }
}

void AVWidget::Render()
{
    MUnit* host = GetMan();
    MUnit* wu = host->GetNode("./Width");
    MDVarGet* wvg = wu->GetObj(wvg);
    MDtGet<Sdata<int>>* wsi = wvg->GetDObj(wsi);
    Sdata<int> wi = 0;
    wsi->DtGet(wi);

    MUnit* hu = host->GetNode("./Height");
    MDVarGet* hvg = hu->GetObj(wvg);
    MDtGet<Sdata<int>>* hsi = hvg->GetDObj(wsi);
    Sdata<int> hi = 0;
    hsi->DtGet(hi);

    int width = 640, height = 480;

    float wc = 0.5*(wi.mData)/width;
    float hc = 0.5*(hi.mData)/height;

    glViewport(0, 0, width, height);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(-wc, -hc);
    glVertex2f(-wc, hc);
    glVertex2f(wc, hc);
    glVertex2f(wc, -hc);
    glEnd();
    glFlush();
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

