
#ifndef __FAP2VIS_AGENTVR_H
#define __FAP2VIS_AGENTVR_H

#include "widget.h"
#include "container.h"
#include "hlayoutl.h"
#include "magentvr.h"


/** @brief Agent visual representation widget
 * Base agent for agents representations
 * */
class AAgentVr : public AVWidget
{
    public:
	static const char* Type() { return "AAgentVr";};
	static string PEType();
	AAgentVr(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AAgentVr();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
    protected:
	static void DrawLine(float x1, float y1, float x2, float y2);
    protected:
	const int K_BFontSize = 18; /**< Base metric: Base font (unit name) size. */
	const int K_BPadding = 3; /**< Base metric: Base padding */
	const int K_LineWidth = 1; /**< Base metric: Line width */
	const int K_MinBodyHeight = 20; /**< Minimum body height */
};

class FTPixmapFont;

/** @brief Unit compact representation widget
 * */
class AUnitCrp : public AAgentVr, public MVrp
{
    public:
	static const char* Type() { return "AUnitCrp";};
	static string PEType();
	AUnitCrp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AUnitCrp();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MSceneElem
	virtual void Render() override;
	virtual bool onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) override;
	// From MVrp
	virtual string MVrp_Mid() const override;
	virtual void SetEnv(MEnv* aEnv) override;
	virtual void SetModel(const string& aMdlUri) override;
	virtual string GetModelUri() const override;
	virtual void SetCrtlBinding(const string& aCtrUri) override {}
    protected:
	// From AVWidget
	virtual void Init() override;
    protected:
	// TODO to have shared font in visual env
	FTPixmapFont* mFont;
	MEnv* mEnv; /*!< Binded env, not owned */
	MUnit* mMdl; /*!< Binded model, not owned */
};

/** @brief Unit detail representation widget
 * */
class AUnitDrp : public AHLayoutL, public MVrp
{
    public:
	static const char* Type() { return "AUnitDrp";};
	static string PEType();
	AUnitDrp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual MIface* DoGetObj(const char *aName) override;
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MSceneElem
	virtual void Render() override;
	// From MVrp
	virtual string MVrp_Mid() const override;
	virtual void SetEnv(MEnv* aEnv) override;
	virtual void SetModel(const string& aMdlUri) override;
	virtual string GetModelUri() const override;
	virtual void SetCrtlBinding(const string& aCtrUri) override;
	// From MDesSyncable
	virtual void Update() override;
    protected:
	void OnInpModelUri();
	void ApplyModelUri();
	// Local
	virtual void CreateRp();
    protected:
	MEnv* mEnv; /*!< Binded env, not owned */
	MUnit* mMdl; /*!< Binded model, not owned */
	string mCtrBnd; /*!< Binding to controller info: URI */
	AAdp::AdpIap mIapModelUri = AAdp::AdpIap(*this, [this]() {OnInpModelUri();}); /*!< Mut input access point: Model Uri */
	TBool mModelUriChanged = ETrue;
};


/** @brief Agents Visual representation view manager
 * */
class AVrpView : public Unit, public MVrpView, public MViewMgr, public MAgent
{
    public:
	static const char* Type() { return "AVrpView";};
	static string PEType();
	AVrpView(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MAgent
	MIface* MAgent_DoGetIface(const string& aName) override;
	// From MVrpView
	virtual void OnCompSelected(const MVrp* aComp) override;
	// From MViewMgr/MIfaceProv
	virtual MIface* MViewMgr_DoGetIface(const string& aName) override;
    protected:
	void CreateRp();
    protected:
	MEnv* mEnv; /*!< Binded env, not owned */
	MUnit* mMdl; /*!< Binded model, not owned */
	string mCtrBnd; /*!< Binding to controller info: URI */
};

#endif

