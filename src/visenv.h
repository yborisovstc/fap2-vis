#ifndef __FAP2VIS_ENV_H
#define __FAP2VIS_ENV_H

#include <unit.h>
#include <syst.h>
#include <des.h>
#include <mdata.h>
#include <mvisenv.h>
#include <mwindow.h>


using namespace std;

class GLFWwindow;

// Visual environment
class AVisEnv:  public ADes, public MVisEnv
{
    public:
	static const char* Type() { return "AVisEnv";};
	static string PEType();
	AVisEnv(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AVisEnv();
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From MElem
	virtual TBool OnCompChanged(MUnit& aComp, const string& aContName = string(), TBool aModif = EFalse);
	// From MVisEnv
	virtual void SetOnIdleHandler(TIdleHandler aHandler) override;
	virtual void Start(void) override;
	void Display(void);
    protected:
	void Construct();
	static const string mCont_Init;
    protected:
	TIdleHandler mIdleHandler;
};


// Top window
class AGWindow: public ADes, public MWindow
{
    public:
	static const char* Type() { return "AGWindow";};
	static string PEType();
	AGWindow(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From MElem
	virtual TBool OnCompChanged(MUnit& aComp, const string& aContName = string(), TBool aModif = EFalse);
	// From MWindow
	virtual void Start(void);
	virtual void GetCursorPos(double& aX, double& aY) override;
	virtual void GetFbSize(int* aW, int* aH) const override;
	virtual string MWindow_Mid() const { return GetUri(0, true);}
	// From MDesSyncable
    public:
	virtual void Update();
	virtual void Confirm() override;
    protected:
	void Construct();
	void Render();
	const GLFWwindow* RawWindow() const { return mWindow;}
	static void onWindowSizeChanged (GLFWwindow *aWnd, int aW, int aH);
	static void onWindowClosed(GLFWwindow *aWnd);
	static void onCursorPosition(GLFWwindow *aWnd, double aX, double aY);
	static void onMouseButton(GLFWwindow *aWnd, int aButton, int aAction, int aMods);
	void onWindowClosed();
	/** @brief Handles cursor position
	 * @param[in] aX, aY screen coordinates relative to the top-left corner of the window
	 * */
	void onCursorPosition(double aX, double aY);
	/** @brief Handles mouse button events
	 * @param[in] aButton - button Id
	 * @param[in] aAction - action: GLFW_PRESS or GLFW_RELEASE
	 * @param[in] aMods - modes
	 * */
	void onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods);
	MUnit* Host();
	static void RegisterInstance(AGWindow* aInst);
	static AGWindow* FindInstance(GLFWwindow* aWnd);
	//<! Window width native settier iface
	MDVarSet* StWidth();
    protected:
	TBool mWndInit;
	GLFWwindow* mWindow;
	// TODO the instances mechanism seems is used just for assosiating GLFW window instance
	// with AGWindow instance. Why don't use glfwSetWindowUserPointer for that?
	static vector<AGWindow*> mInstances; //!< Register of instances
};

inline MUnit* AGWindow::Host() {return iMan == NULL ? NULL : iMan;}

/** @brief Launcher agent
 * */
class AVDesLauncher: public ADesLauncher
{
    public:
	static const char* Type() { return "AVDesLauncher";};
	static string PEType();
	AVDesLauncher(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From ADesLauncher
	virtual void OnIdle() override;
};

#endif

