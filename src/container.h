
#ifndef __FAP2VIS_CONTAITER_H
#define __FAP2VIS_CONTAITER_H

#include <map>
#include <mdes.h>
#include <mdata.h>
//#include <data.h>

#include "mcontainer.h"
#include "widget.h"


/** @brief Base widgets containter agent
 * */
class AVContainer: public AVWidget, public MDesInpObserver_Imd
{
    public:
	/** @brief Id of widget parameter */
	using TWdgPar = enum {
	    E_AlcW,
		E_AlcH,
		E_AlcX,
		E_AlcY,
		E_RqsW,
		E_RqsH,
	};
    public:
	/** @brief Widget access point
	 * Bridge representing  composer to widget requestors
	 * Actually this is container access point for widget
	 * */
	class WidgetAp: public MDesInpObserver_Imd {
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
		WidgetAp(AVContainer& aHost, const string& aSlot): mHost(aHost), mSlot(aSlot) {}
		WidgetAp(const WidgetAp& aSrc): mHost(aSrc.mHost), mSlot(aSrc.mSlot) {}
	    protected:
		// From MDesInpObserver
		virtual void OnInpUpdated() override {}
		virtual MIface* MDesInpObserver_Call(const string& aSpec, string& aRes) override { return NULL;}
		virtual string MDesInpObserver_Mid() const override {return string();}
	    protected:
		AVContainer& mHost;
		string mSlot;
	    public:
		WdgPap mAlcW = WdgPap(this, E_AlcW);
		WdgPap mAlcH = WdgPap(this, E_AlcH);
		WdgPap mAlcX = WdgPap(this, E_AlcX);
		WdgPap mAlcY = WdgPap(this, E_AlcY);
	};
    public:
/** @brief Transition of requisition
 * */
	class TrReq: public MDVarGet, public MDtGet<Sdata<int>> {
	    public:
		TrReq(AVContainer* aHost, bool aW): mHost(aHost), mW(aW) {}
		// From MDVarGet
		virtual void *DoGetDObj(const char *aName) override;
		virtual string VarGetIfid() override {return string();}
		// From MDtGet
		virtual void DtGet(Sdata<int>& aData) override;
	    protected:
		AVContainer* mHost;
		bool mW; /*!< Sign of width transtion, otherwise height */
	};
    public:
	/** @brief Widget apps registry, key - slot name
	 * */
	using TWAps = std::map<string, WidgetAp>;

	/** @brief Widget DVarGet providers
	 * */
	using TWDvgProv = std::map<string, MDVarGet*>;
    public:
	static const char* Type() { return "AVContainer";};
	static string PEType();
	AVContainer(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
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
	virtual int GetSlotId(const string& aSlotName) const;
	virtual string GetSlotName(int aSlotId) const;
	virtual int GetNextSlot(int aSlotId) const;
	virtual int GetLastSlot();
	/** @brief Gets container component by name */
	virtual MUnit* GetCntComp(const string& aCompName);
	/** @brief Notifies widgets of inp updated
	 * param[inp] aOutUri container "output" assosiated to widget input
	 * */
	virtual void NotifyWidgetOnInpUpdated(const string& aOutUri);
	virtual void AddComp(const string& aName, const string& aType, const string& aHint = string());
    public:
	virtual void Update();
    protected:
	virtual void Init();
	/** @brief Gets input of comps requistion **/
	MUnit* GetCompRqsInp(bool aW);
	/** @brief Gets requisision **/
	MUnit* GetRqsInp(bool aW);
	/** @brief Gets component requisision **/
	int GetCompRqs(const string& aSlot, bool aW);
    protected:
	TWAps mWap; /**< Widget access points */
	TWDvgProv mRqsW; /**< Widget Width Requisition MDVarGet providers  */
	TWDvgProv mRqsH; /**< Widget Hight Requisition MDVarGet providers  */
	int mPadding = 0; /*!< Slot padding */
	TrReq mTrRecW = TrReq(this, true); /*!< Transition of requisition */
	TrReq mTrRecH = TrReq(this, false); /*!< Transition of requisition */
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
 * Connection point like node customizing IRM path to widget outputs
 * */
class AVCpsCp: public Unit
{
    public:
	static const char* Type() { return "AVCpsCp";};
	static string PEType();
	AVCpsCp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
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

