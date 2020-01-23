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
	// From MDesSyncable
    public:
	virtual void Update();
	virtual void Confirm() override;
    protected:
	void Construct();
	void Render();
	const GLFWwindow* RawWindow() const { return mWindow;}
	static void onWindowSizeChanged (GLFWwindow *aWnd, int aW, int aH);
	MUnit* Host();
	static void RegisterInstance(AGWindow* aInst);
	static AGWindow* FindInstance(GLFWwindow* aWnd);
	//<! Window width native settier iface
	MDVarSet* StWidth();
    protected:
	TBool mWndInit;
	GLFWwindow* mWindow;
	static vector<AGWindow*> mInstances; //!< Register of instances
};

inline MUnit* AGWindow::Host() {return iMan == NULL ? NULL : iMan;}



#endif

