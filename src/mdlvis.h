#ifndef __FAP2VIS_MDLVIS_H
#define __FAP2VIS_MDLVIS_H

#include <des.h>

#include "mmdlvis.h"

using namespace std;


/** @brier Model for visual representation agent
 * */
class AMdlVis: public ADes, public MMdlVis
{
    public:
	static const char* Type() { return "AMdlVis";};
	static string PEType();
	AMdlVis(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From MMdlVis
	virtual MIfProv* IfProv() override;
    protected:
	void Construct();
};

#endif


