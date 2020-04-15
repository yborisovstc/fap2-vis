
#ifndef __FAP2VIS_CONTAITER_H
#define __FAP2VIS_CONTAITER_H

#include <map>
#include <mdes.h>
#include <mdata.h>
//#include <data.h>

#include "mcontainer.h"
#include "widget.h"


/** @brief Base widgets containter agent
 * It contains set (mWap) of allocation providers specific for each component (widget)
 * */
class AVContainerBase: public AVWidget, public MDesInpObserver_Imd
{
    public:
	/** @brief Id of widget parameter */
	using TWdgPar = enum { E_AlcW, E_AlcH, E_AlcX, E_AlcY };
    public:
	/** @brief Widget access point
	 * Bridge representing  composer to widget allocation
	 * Actually this is container access point for widget
	 * */
	class WidgetAp {
	    public:
		/** @brief Widget parameter access point
		 * Represents widget paremeter like allocation.width
		 * */
		class WdgPap: public MDVarGet, public MDtGet<Sdata<int>> {
		    public:
			WdgPap(const WdgPap& aSrc): mHost(aSrc.mHost), mPar(aSrc.mPar) {}
			WdgPap(WidgetAp* aHost, TWdgPar aPar): mHost(aHost), mPar(aPar) {}
			void SetHost(WidgetAp* aHost) {mHost = aHost;}
			// From MDVarGet
			virtual void *DoGetDObj(const char *aName) override;
			virtual string VarGetIfid() override {return string();}
			// From MDtGet
			virtual void DtGet(Sdata<int>& aData) override;
		    protected:
			WidgetAp* mHost;
			TWdgPar mPar;
		};
	    public:
		friend WdgPap;
		WidgetAp(AVContainerBase& aHost, const string& aSlot): mHost(aHost), mSlot(aSlot) {}
		WidgetAp(const WidgetAp& aSrc): mHost(aSrc.mHost), mSlot(aSrc.mSlot) {}
	    protected:
		AVContainerBase& mHost;
		string mSlot;
	    public:
		WdgPap mAlcW = WdgPap(this, E_AlcW);
		WdgPap mAlcH = WdgPap(this, E_AlcH);
		WdgPap mAlcX = WdgPap(this, E_AlcX);
		WdgPap mAlcY = WdgPap(this, E_AlcY);
	};
    public:
	/** @brief Widget apps registry, key - slot name */
	using TWAps = std::map<string, WidgetAp>;
    public:
	static const char* Type() { return "AVContainerBase";};
	static string PEType();
	AVContainerBase(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
	virtual void onSeCursorPosition(double aX, double aY) override;
	virtual bool onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) override;
	// From MDesInpObserver
	virtual void OnInpUpdated() override;
	virtual MIface* MDesInpObserver_Call(const string& aSpec, string& aRes) override;
	virtual string MDesInpObserver_Mid() const override;
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// Local
	virtual int GetComposedData(const string& aSlotName, TWdgPar aPar) = 0; 
	virtual int GetSlotId(const string& aSlotName) const = 0;
	virtual string GetSlotName(int aSlotId) const = 0;
	virtual int GetNextSlot(int aSlotId) const = 0;
	virtual int GetLastSlot() = 0;
	/** @brief Gets container component by name */
	virtual MUnit* GetCntComp(const string& aCompName) = 0;
	/** @brief Notifies widgets of inp updated
	 * param[inp] aOutUri container "output" assosiated to widget input
	 * */
	virtual void NotifyWidgetOnInpUpdated(const string& aOutUri);
	/** @brief Notifies requisitions of inp updated
	 * param[inp] aW sign of W requisition, otherwise H
	 * */
	void NotifyReqsOnInpUpdated(bool aW);
	virtual void AddComp(const string& aName, const string& aType, const string& aHint = string()) = 0;
    public:
	virtual void Update();
    protected:
	inline MUnit* Host() { return iMan;}
	virtual void Init();
	/** @brief Gets input of comps requistion **/
	MUnit* GetCompRqsInp(bool aW);
	/** @brief Gets requisision **/
	MUnit* GetRqsInp(bool aW);
	/** @brief Gets component requisision **/
	virtual int GetCompRqs(const string& aSlot, bool aW) = 0;
    protected:
	TWAps mWap; /**< Widget access points */
	int mPadding = 0; /*!< Slot padding */
};

/** @brief Widgets containter agent using approach of embedding widgets in slot
 * */
class AVContainer: public AVContainerBase
{
    public:
	static const char* Type() { return "AVContainer";};
	static string PEType();
	AVContainer(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// Local
	virtual int GetSlotId(const string& aSlotName) const override;
	virtual string GetSlotName(int aSlotId) const override;
	virtual int GetNextSlot(int aSlotId) const override;
	virtual int GetLastSlot() override;
	virtual MUnit* GetCntComp(const string& aCompName) override;
	virtual void AddComp(const string& aName, const string& aType, const string& aHint = string()) override;
    protected:
	virtual int GetCompRqs(const string& aSlot, bool aW) override;
};


/** @brief Container's slot
 * */
class AVSlot: public Vertu, public MVCslot
{
    public:
	static const char* Type() { return "AVSlot";};
	static string PEType();
	AVSlot(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
};


/** @brief Container compositor input
 * Binided to components outputs
 * */
class AVCpsCpInp: public ConnPointMcu
{
    public:
	static const char* Type() { return "AVCpsCpInp";};
	static string PEType();
	AVCpsCpInp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
};

/** @brief Container compositor output
 * Binided to components inputs
 * */
class AVCpsCpOut: public ConnPointMcu
{
    public:
	static const char* Type() { return "AVCpsCpOut";};
	static string PEType();
	AVCpsCpOut(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
};

/** @brief Transition of container requistion state, base */
class TrCntReq: public ATrcVar, public MDtGet<Sdata<int> >  {
    public:
	static const char* Type() { return "TrCntReq";};
	static string PEType();
	TrCntReq(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MDVarGet
	virtual void *DoGetDObj(const char *aName) override;
};


/** @brief Transition of container requistion state, sum of inputs
 * */
class TrReqSum: public TrCntReq  {
    public:
	static const char* Type() { return "TrReqSum";};
	static string PEType();
	TrReqSum(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MDtGet<Sdata<int>>
	virtual void DtGet(Sdata<int>& aData) override;
};

/** @brief Transition of container requistion state, Max of inputs
 * */
class TrReqMax: public TrCntReq  {
    public:
	static const char* Type() { return "TrReqMax";};
	static string PEType();
	TrReqMax(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MDtGet<Sdata<int>>
	virtual void DtGet(Sdata<int>& aData) override;
};



/** @brief Horisontal layout containter agent
 * */
class AVHLayout: public AVContainer
{
    public:
	static const char* Type() { return "AVHLayout";};
	static string PEType();
	AVHLayout(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
	// From AVContainer
	virtual int GetComposedData(const string& aSlotName, TWdgPar aPar) override; 
    public:
	virtual void Update();
    protected:
	virtual void Init();
};


#endif // __FAP2VIS_CONTAITER_H

