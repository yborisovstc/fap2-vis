#ifndef __FAP2VIS_WIDGET_H
#define __FAP2VIS_WIDGET_H

#include "mwidget.h"
#include <elem.h>
#include <mdes.h>
#include <mdata.h>
#include <mwidget.h>
#include <gdk/gdk.h>


using namespace std;

// Visual environment
class AVisEnv:  public Elem, public MDesSyncable, public MDesObserver, public MGdkEventHandler
{
    public:
	static const char* Type() { return "AVisEnv";};
	static string PEType();
	AVisEnv(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AVisEnv(MElem* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AVisEnv();
	// From Base
	virtual void *DoGetObj(const char *aName);
	// From MElem
	virtual TBool OnCompChanged(MElem& aComp, const string& aContName = string());
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
	void Construct();
    private:
	TBool iActive;
	TBool iUpdated;
};


// Gdk events handling mediator: just creates a "net" of MGdkEventHandler instances 
class AGehMediator: public Elem, public MGdkEventHandler
{
    public:
	static const char* Type() { return "AGehMediator";};
	static string PEType();
	AGehMediator(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AGehMediator(MElem* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual void *DoGetObj(const char *aName);
	// Iface provider
	virtual void UpdateIfi(const string& aName, const RqContext* aCtx);
	// From MGdkEventHandler
	virtual void OnEvent(GdkEvent* event);
};

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

// Top window
class AGWindow: public AGWidget
{
    public:
	static const char* Type() { return "AGWindow";};
	static string PEType();
	AGWindow(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AGWindow(MElem* aMan = NULL, MEnv* aEnv = NULL);
    public:
	// From MDesSyncable
	virtual void Update();
	// From MGWidgetComp
	virtual void OnGdkEvent(GdkEvent* event);
    protected:
	void Construct();
	virtual void OnUpdated_X(int aOldData);
	virtual void OnUpdated_Y(int aOldData);
	virtual void OnUpdated_W(int aOldData);
	virtual void OnUpdated_H(int aOldData);
    protected:
	GdkWindow* mWindow;
	TBool mWndInit;
};

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

