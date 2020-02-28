#ifndef __FAP2VIS_WIDGET_H
#define __FAP2VIS_WIDGET_H

#include "mwidget.h"

#include <array>
#include <des.h>

#include <mscel.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>


/** @brief Widget base agent
 *
 * */
// TODO Use unit based DES agent as a base
class AVWidget : public ADes, public MSceneElem, public MACompsObserver
{
    public:
	using TColor = struct {float r, g, b, a;};
    public:
	static const char* Type() { return "AVWidget";};
	static string PEType();
	AVWidget(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
	virtual void onSeCursorPosition(double aX, double aY) override;
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MDesSyncable
	virtual void Update();
    protected:
	virtual void Init();
    protected:
	int GetParInt(const string& aUri);
	static void CheckGlErrors();
    protected:
	bool mIsInitialised = false;
	GLuint mProgram;
	GLint mMvpLocation;
	TColor mBgColor;
};

#endif // __FAP2VIS_WIDGET_H
