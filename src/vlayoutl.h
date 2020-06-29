
#ifndef __FAP2VIS_VLAYOUTL_H
#define __FAP2VIS_VLAYOUTL_H

#include <map>

#include "containerl.h"

/** @brief Vertical layout agent using approach of widgets linked to slot
 * */
class AVLayoutL: public ALinearLayout
{
    public:
	static const char* Type() { return "AVLayoutL";};
	static string PEType();
	AVLayoutL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From AVContainer
	virtual string GetSlotType() override;
};

#endif // __FAP2VIS_VLAYOUTL_H

