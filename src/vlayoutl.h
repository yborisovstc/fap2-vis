
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
	// From AVContainer
	virtual void AddWidget(const string& aName, const string& aType, const string& aHint = string()) override;
};

#endif // __FAP2VIS_VLAYOUTL_H

