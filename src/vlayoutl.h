
#ifndef __FAP2VIS_VLAYOUTL_H
#define __FAP2VIS_VLAYOUTL_H

#include <map>

#include "containerl.h"

/** @brief Vertical layout agent using approach of widgets linked to slot
 * */
class AVLayoutL: public AVContainerL
{
    public:
	static const char* Type() { return "AVLayoutL";};
	static string PEType();
	AVLayoutL(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
};

#endif // __FAP2VIS_VLAYOUTL_H

