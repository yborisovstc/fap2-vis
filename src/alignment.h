
#ifndef __FAP2VIS_ALIGNMENT_H
#define __FAP2VIS_ALIGNMENT_H

#include "container.h"


/** @brief Alignment container
 * */
class AAlignment: public AVContainer
{
    public:
	static const char* Type() { return "AAlignment";};
	static string PEType();
	AAlignment(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
	// From AVContainer
	virtual int GetComposedData(const string& aSlotName, TWdgPar aPar) override; 
    protected:
	virtual void Init();
};

#endif // __FAP2VIS_VLAYOUT_H

