#ifndef __FAP2VIS_VISREPR_H
#define __FAP2VIS_VISREPR_H

#include "mvisrepr.h"

#include <munit.h>
#include <unit.h>

using namespace std;

class MEnv;

/** @brier Visual representation agent
 * */
class AVisRepr: public Unit, public MAgent, public MVisRepr
{
    public:
	static const char* Type() { return "AVisRepr";};
	static string PEType();
	AVisRepr(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From Unit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MVisRepr
	virtual MUnit* IfProv() override;
	// From MAgent
	MIface* MAgent_DoGetIface(const string& aName) override;
};

#endif // __FAP2VIS_VISREPR_H


