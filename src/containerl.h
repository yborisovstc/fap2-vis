
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
class AVContainerL: public AVWidget
{
    public:
	static const char* Type() { return "AVContainerL";};
	static string PEType();
	AVContainerL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MUnit
	virtual MIface* DoGetObj(const char *aName) override;
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
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

#endif // __FAP2VIS_CONTAITER_H

