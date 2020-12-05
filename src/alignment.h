
#ifndef __FAP2VIS_ALIGNMENT_H
#define __FAP2VIS_ALIGNMENT_H

#include "containerl.h"


/** @brief Alignment container
 * */
class AAlignment: public AVContainerL
{
    public:
	static const char* Type() { return "AAlignment";};
	static string PEType();
	AAlignment(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName = string()) override;
	// From MContainer
	virtual MUnit* GetSlotByPos(const TPos& aPos) override;
	virtual MUnit* AddWidget(const string& aName, const string& aType, const string& aHint = string()) override;
	virtual void UpdateCompNames() override;
};

#endif // __FAP2VIS_VLAYOUT_H

