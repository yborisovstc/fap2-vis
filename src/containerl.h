
#ifndef __FAP2VIS_CONTAINERL_H
#define __FAP2VIS_CONTAINERL_H

#include <map>
#include <mdes.h>
#include <mdata.h>
//#include <data.h>

#include "mcontainer.h"
#include "widget.h"

/** @brief Widgets containter agent using approach of widgets linked to slot
 * With this approach each widget is assosiates to corresponding slot but not embedded to it
 * This container doesn't provide widgets allocation by itself but delegates it to slots
 * Each slot can have its own rules for allocating assosiated widget
 * */
class AVContainerL: public AVWidget, public MContainer
{
    public:
	using TCmpNames = AMunitAdp::TCmpNames;
	friend AAdp::AdpMagObs<AVContainerL>;
    public:
	static const char* Type() { return "AVContainerL";};
	static string PEType();
	AVContainerL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AVContainerL(); 
	// From MUnit
	virtual MIface* DoGetObj(const char *aName) override;
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MSceneElem
	virtual void Render() override;
	virtual bool onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) override;
	// From MContainer
	virtual MUnit* AddWidget(const string& aName, const string& aType, const string& aHint = string()) override;
	virtual MUnit* InsertWidget(const string& aName, const string& aType, const TPos& aPos) override;
	virtual bool RmWidget(int aSlotPos, const string& aHint = string()) override;
	virtual TPos LastPos() const override;
	// From MDesSyncable
	virtual void Confirm() override;
	// Local
	virtual string GetSlotType();
	virtual MUnit* AppendSlot(MUnit* aSlot);
	virtual MUnit* InsertSlot(MUnit* aSlot, const TPos& aPos);
	virtual void RmSlot(MUnit* aSlot);
	virtual int GetLastSlotId();
	virtual int GetSlotId(const string& aSlotName) const;
	virtual string GetSlotName(int aSlotId) const;
	virtual MUnit* GetPrevSlotCp(MUnit* aSlot);
	virtual MUnit* GetNextSlotCp(MUnit* aSlot);
	virtual MUnit* GetNextSlot(MUnit* aSlot);
	virtual MUnit* GetSlotByPos(const TPos& aPos) override;
	virtual TPos PrevPos(const TPos& aPos) const;
	virtual TPos NextPos(const TPos& aPos) const;
	virtual MUnit* GetWidgetBySlot(MUnit* aSlot);
    protected:
	// Local
	void MutAddWidget(const NTuple& aData);
	void MutRmWidget(const Sdata<int>& aData);
	void GetCompsCount(Sdata<TInt>& aData);
	void GetCompNames(AMunitAdp::TCmpNames& aData) { aData = mCompNames;}
	void OnMutAddWdg();
	void OnMutRmWdg();
	virtual void UpdateCompNames() {}
	MUnit* Host() { return iMan;}
	/** @brief Notifies dependencies of input updated */
	void NotifyInpsUpdated(MUnit* aCp);
	// For managed agent observer
	virtual void OnMagCompDeleting(const MUnit* aComp, TBool aSoft = ETrue, TBool aModif = EFalse);
	virtual void OnMagCompAdding(const MUnit* aComp, TBool aModif = EFalse);
	virtual TBool OnMagCompChanged(const MUnit* aComp, const string& aContName = string(), TBool aModif = EFalse);
	virtual TBool OnMagChanged(const MUnit* aComp);
	virtual TBool OnMagCompRenamed(const MUnit* aComp, const string& aOldName);
	virtual void OnMagCompMutated(const MUnit* aNode);
	virtual void OnMagError(const MUnit* aComp);
    protected:
	static const TPos KPosFirst;
	static const TPos KPosEnd;
	NTuple mMutAddWidget;     /*!< Mutation: append component */
	Sdata<TInt> mMutRmWidget;     /*!< Mutation: remove component */
	MUnit* mMag = NULL;       /*!< Managed object, host */
	TCmpNames mCompNames;     /*!< Component names, observable data */
	TBool mCompNamesUpdated = ETrue;
	// Comps count param adapter. Even if the count can be get via comp names vector we support separate param for convenience
	AAdp::AdpPap<int> mApCmpCount = AAdp::AdpPap<int>(*this, [this](Sdata<TInt>& aData) {GetCompsCount(aData);}); /*!< Comps count access point */
	AAdp::AdpPapB<TCmpNames> mApCmpNames = AAdp::AdpPapB<TCmpNames>([this](TCmpNames& aData) {GetCompNames(aData);}); /*!< Comp names access point */
	AAdp::AdpMagObs<AVContainerL> mMagObs = AAdp::AdpMagObs<AVContainerL>(this); /*!< Managed agent observer */
	AAdp::AdpIap mIapMutAddWdt = AAdp::AdpIap(*this, [this]() {OnMutAddWdg();}); /*!< Mut Add_Widget input access point */
	AAdp::AdpIap mIapMutRmWdt = AAdp::AdpIap(*this, [this]() {OnMutRmWdg();}); /*!< Mut Remove_Widget input access point */
};

/** @brief Container's slot base using approach of widghet to slot assosiation via link
 * */
// TODO Do we need specific agent for slot?
class AVSlotL: public Syst, public MVCslot
{
    public:
	static const char* Type() { return "AVSlotL";};
	static string PEType();
	AVSlotL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
};

/** @brief Linear layout base agent
 * */
class ALinearLayout: public AVContainerL
{
    public:
	static const char* Type() { return "ALinearLayout";};
	static string PEType();
	ALinearLayout(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From AVContainer
	virtual MUnit* AppendSlot(MUnit* aSlot) override;
	virtual MUnit* InsertSlot(MUnit* aSlot, const TPos& aPos) override;
	virtual void RmSlot(MUnit* aSlot) override;
	virtual MUnit* GetSlotByPos(const TPos& aPos) override;
	virtual MUnit* GetPrevSlotCp(MUnit* aSlot) override;
	virtual MUnit* GetNextSlotCp(MUnit* aSlot) override;
	virtual MUnit* GetNextSlot(MUnit* aSlot) override;
	virtual TPos PrevPos(const TPos& aPos) const override;
	virtual TPos NextPos(const TPos& aPos) const override;
	virtual void UpdateCompNames() override;
    protected:
	MUnit* GetLastSlot();
};

;

#endif // __FAP2VIS_CONTAITER_H

