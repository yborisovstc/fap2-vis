#ifndef __FAP2VIS_WIDGET_H
#define __FAP2VIS_WIDGET_H

#include "mwidget.h"

#include <array>
#include <des.h>

#include <mscel.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>


class MWindow;

/** @brief Widget base agent
 *
 * */
// TODO Use unit based DES agent as a base
class AVWidget : public ADes, public MSceneElem, public MACompsObserver, public MVCcomp
{
    public:
	using TColor = struct {float r, g, b, a;};
    public:
	static const char* Type() { return "AVWidget";};
	static string PEType();
	AVWidget(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual string MSceneElem_Mid() const { return GetUri(0, ETrue);}
	virtual void Render() override;
	virtual void onSeCursorPosition(double aX, double aY) override;
	virtual bool onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) override;
	virtual void getWndCoord(int aInpX, int aInpY, int& aOutX, int& aOutY) override;
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MDesSyncable
	virtual void Update() override;
	virtual void Confirm() override;
	// From MVCcomp
	virtual string MVCcomp_Mid() const {return GetUri(NULL, true);}
    protected:
	virtual void Init();
	/** @brief Handles cursor position change
	 * @param[in] aX, aY  Pos widget coordinates
	 * */
	virtual void onWdgCursorPos(int aX, int aY);
	static void DrawLine(float x1, float y1, float x2, float y2);
    protected:
	int GetParInt(const string& aUri);
	static void CheckGlErrors();
	void GetCursorPosition(double& aX, double& aY);
	bool IsInnerWidgetPos(double aX, double aY);
	int WndX(int aX);
	int WndY(int aY);
	MWindow* Wnd();
    protected:
	bool mIsInitialised = false;
	GLuint mProgram;
	GLint mMvpLocation;
	TColor mBgColor;
	TColor mFgColor;
	static const string KCnt_BgColor;
	static const string KCnt_FgColor;
	static const string KCnt_FontPath;
};

#endif // __FAP2VIS_WIDGET_H
