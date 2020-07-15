
#ifndef __FAP2VIS_SCROLLEDBOX_H
#define __FAP2VIS_SCROLLEDBOX_H

#include "container.h"


/** @brief Scrolled box
 * */
class AScrBox: public AVContainer
{
    public:
	static const char* Type() { return "AScrBox";};
	static string PEType();
	AScrBox(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit* aContext, MUnit* aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
	// From AVContainer
	virtual int GetComposedData(const string& aSlotName, TWdgPar aPar) override; 
    protected:
	virtual void Init();
};

#endif // __FAP2VIS_SCROLLEDBOX_H

