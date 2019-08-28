#ifndef __FAP2VIS_MDLVIS_H
#define __FAP2VIS_MDLVIS_H

#include <des.h>

#include "mmdlvis.h"

using namespace std;


/** @brier Model for visual representation agent
 * */
class AMdlVis: public Unit, public MAgent, public MMdlVis
{
    public:
	static const char* Type() { return "AMdlVis";};
	static string PEType();
	AMdlVis(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From Unit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MMdlVis
	virtual MUnit* IfProv() override;
	// From MAgent
	MIface* MAgent_DoGetIface(const string& aName) override;
    protected:
	void Construct();
};

#endif


