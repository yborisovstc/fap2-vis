
#ifndef __FAP2VIS_LABEL_H
#define __FAP2VIS_LABEL_H

#include "widget.h"


/** @brief Label widget agent
 * */
class AVLabel : public AVWidget
{
    public:
	static const char* Type() { return "AVLabel";};
	static string PEType();
	AVLabel(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AVLabel();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
};

#endif

