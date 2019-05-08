#ifndef __FAP2VIS_WIDGET_H
#define __FAP2VIS_WIDGET_H

#include <unit.h>
#include <syst.h>
#include <des.h>
#include <mdata.h>
#include <mvisenv.h>


using namespace std;

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


#if 0
class AGWidget: public Elem, public MGWidget, public MGWidgetComp, public MGWidgetOwner, public MACompsObserver, public MDesSyncable, public MDesObserver
{
    public:
	typedef map<TState, string>  tStatesMap;
    protected:
	// Data provider base
	class DataProv {
	    public:
		void SetHost(AGWidget* aHost)  {mHost = aHost;};
	    protected:
		AGWidget* mHost;
	};
	// Data provider for ButtonPress GdkEventButton
	class EventButtonProv: public DataProv, public MDVarGet, public MDtGet<NTuple> {
	    public:
	    // From MDVarGet
	    virtual string VarGetIfid();
	    virtual void *DoGetDObj(const char *aName);
	    // From MDtGet
	    virtual void DtGet(NTuple& aData);
	};
	// Data provider for ButtonRelease GdkEventButton
	class EventButtonReleaseProv: public DataProv, public MDVarGet, public MDtGet<NTuple> {
	    public:
	    // From MDVarGet
	    virtual string VarGetIfid();
	    virtual void *DoGetDObj(const char *aName);
	    // From MDtGet
	    virtual void DtGet(NTuple& aData);
	};
	// Data provider for GdkEventMotion
	class EventMotionProv: public DataProv, public MDVarGet, public MDtGet<NTuple> {
	    public:
	    // From MDVarGet
	    virtual string VarGetIfid();
	    virtual void *DoGetDObj(const char *aName);
	    // From MDtGet
	    virtual void DtGet(NTuple& aData);
	};
	// Data provider for Allocation
	class AllocationProv: public DataProv, public MDVarGet, public MDtGet<NTuple> {
	    public:
	    // From MDVarGet
	    virtual string VarGetIfid();
	    virtual void *DoGetDObj(const char *aName);
	    // From MDtGet
	    virtual void DtGet(NTuple& aData);
	};
	// Data provider for SizeRequest
	class SizeRequestProv: public DataProv, public MDVarGet, public MDtGet<NTuple> {
	    public:
	    // From MDVarGet
	    virtual string VarGetIfid();
	    virtual void *DoGetDObj(const char *aName);
	    // From MDtGet
	    virtual void DtGet(NTuple& aData);
	};
    public:
	static const char* Type() { return "AGWidget";};
	static string PEType();
	AGWidget(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AGWidget(MElem* aMan = NULL, MEnv* aEnv = NULL);
	void Construct();
	// From MGWidget
	virtual MElem* GetLayoutHint() const;
	// From MGWidgetOwner
	virtual GdkWindow* GetWindow();
	// From MGWidgetComp
	virtual void OnGdkEvent(GdkEvent* event);
	// From MACompsObserver
	virtual TBool HandleCompChanged(MElem& aContext, MElem& aComp, const string& aName=string());
	// From MDesObserver
	virtual void OnUpdated();
	virtual void OnActivated();
	// From MDesSyncable
	virtual void Update();
	virtual void Confirm();
	virtual TBool IsUpdated();
	virtual void SetUpdated();
	virtual void ResetUpdated();
	virtual TBool IsActive();
	virtual void SetActive();
	virtual void ResetActive();
    protected:
	void GetBtnPressEvent(NTuple& aData) { aData = mBtnPressEvt;};
	void GetBtnReleaseEvent(NTuple& aData) { aData = mBtnReleaseEvt;};
	void GetMotionEvent(NTuple& aData) { aData = mMotionEvt;};
	void ActivateDeps(const string& aUri);
	// From Base
	virtual void *DoGetObj(const char *aName);
	// Ifaces cache
	virtual void UpdateIfi(const string& aName, const RqContext* aCtx);
	// Widget events handling
	bool OnButtonPress(GdkEventButton* aEvent);
	bool OnButtonRelease(GdkEventButton* aEvent);
	bool OnMotion(GdkEventMotion* aEvent);
    protected:
	virtual void OnUpdated_X(int aOldData);
	virtual void OnUpdated_Y(int aOldData);
	virtual void OnUpdated_W(int aOldData);
	virtual void OnUpdated_H(int aOldData);
	virtual void OnUpdated_State(TState mOld);
	virtual bool HandleButtonPress(GdkEventButton* aEvent);
	virtual bool HandleButtonRelease(GdkEventButton* aEvent);
	virtual bool HandleMotion(GdkEventMotion* aEvent);
    protected:
	MElem* Host();
    private:
	bool GetDataInt(const string& aInpUri, int& aData);
	bool GetInpState(const string& aInpUri, TState& aData);
	MGWidgetOwner* GetOwner();
    protected:
	TBool iActive;
	TBool iUpdated;
	EventButtonProv mBtnPressEvtProv;
	EventButtonReleaseProv mBtnReleaseEvtProv;
	EventMotionProv mMotionEvtProv;
	AllocationProv mAllocationProv;
	AllocationProv mSizeRequestProv;
	int iY;
	int iX;
	int iW;
	int iH;
	NTuple mBtnPressEvt;
	NTuple mBtnReleaseEvt;
	NTuple mMotionEvt;
	NTuple mAllocation;
	TState mState;
	NTuple mSizeRequest;
	static bool mInit;
	static tStatesMap  mStatesMap;
};
#endif

#if 0
// Top window
class AGWindow: public Unit, public MDesSyncable_Imd
{
    public:
	static const char* Type() { return "AGWindow";};
	static string PEType();
	AGWindow(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
    public:
	// From MDesSyncable
	virtual void Update();
	virtual void Confirm() {}
	virtual TBool IsUpdated() {return EFalse;}
	virtual void SetUpdated() {}
	virtual void ResetUpdated() {}
	virtual TBool IsActive() {return EFalse;}
	virtual void SetActive() {}
	virtual void ResetActive() {}
	virtual MIface* MDesSyncable_Call(const string& aSpec, string& aRes) {return NULL;}
	virtual string MDesSyncable_Mid() const override { return string();}

    protected:
	void Construct();
    protected:
	GdkWindow* mWindow;
	TBool mWndInit;
};
#endif

// Top window
class AGWindow: public ADes
{
    public:
	static const char* Type() { return "AGWindow";};
	static string PEType();
	AGWindow(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MDesSyncable
    public:
	virtual void Update();
    protected:
	void Construct();
    protected:
	TBool mWndInit;
};

#if 0
// Main state that manages GDK window
class AStateWnd:  public Elem, public MDesSyncable, public MDesObserver, public MGdkEventHandler
{
    public:
	static const char* Type() { return "AStateWnd";};
	static string PEType();
	AStateWnd(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AStateWnd(MElem* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AStateWnd();
	// From Base
	virtual void *DoGetObj(const char *aName);
	// From MDesSyncable
	virtual void Update();
	virtual void Confirm();
	virtual TBool IsUpdated();
	virtual void SetUpdated();
	virtual void ResetUpdated();
	virtual TBool IsActive();
	virtual void SetActive();
	virtual void ResetActive();
	// From MDesObserver
	virtual void OnUpdated();
	virtual void OnActivated();
	// Iface provider
	virtual void UpdateIfi(const string& aName, const RqContext* aCtx);
	// From MGdkEventHandler
	virtual void OnEvent(GdkEvent* event);
    protected:
	MElem* Host();
	void Construct();
	bool GetDataInt(const string& aInpUri, int& aData);
	bool GetDataString(const string& aInpUri, string& aData);
    private:
	TBool iActive;
	TBool iUpdated;
	GdkWindow* iWnd;
	TBool mInit;
	string mTitle;
};
#endif

#endif

