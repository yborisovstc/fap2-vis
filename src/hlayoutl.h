
#ifndef __FAP2VIS_HLAYOUTL_H
#define __FAP2VIS_HLAYOUTL_H

#include <map>

#include "containerl.h"

/** @brief Horizontal layout agent using approach of widgets linked to slot
 * */
class AHLayoutL: public ALinearLayout
{
    public:
	static const char* Type() { return "AHLayoutL";};
	static string PEType();
	AHLayoutL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From AVContainer
	virtual string GetSlotType() override;
};

#endif // __FAP2VIS_HLAYOUTL_H

