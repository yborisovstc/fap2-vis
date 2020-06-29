
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
	static const char* Type() { return "AVContainerL";};
	static string PEType();
	AVContainerL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual MIface* DoGetObj(const char *aName) override;
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MSceneElem
	virtual void Render() override;
	virtual bool onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) override;
	// From MContainer
	virtual MUnit* AddWidget(const string& aName, const string& aType, const string& aHint = string()) override;
	virtual MUnit* InsertWidget(const string& aName, const string& aType, const TPos& aPos) override;
	virtual TPos LastPos() const override;
	// Local
	virtual string GetSlotType();
	virtual MUnit* AppendSlot(MUnit* aSlot);
	virtual MUnit* InsertSlot(MUnit* aSlot, const TPos& aPos);
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
	static const TPos KPosFirst;
	static const TPos KPosEnd;
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
	virtual MUnit* GetSlotByPos(const TPos& aPos) override;
	virtual MUnit* GetPrevSlotCp(MUnit* aSlot) override;
	virtual MUnit* GetNextSlotCp(MUnit* aSlot) override;
	virtual MUnit* GetNextSlot(MUnit* aSlot) override;
	virtual TPos PrevPos(const TPos& aPos) const override;
	virtual TPos NextPos(const TPos& aPos) const override;
    protected:
	MUnit* GetLastSlot();
};

;

#endif // __FAP2VIS_CONTAITER_H

