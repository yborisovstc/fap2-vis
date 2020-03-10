
#ifndef __FAP2VIS_BUTTON_H
#define __FAP2VIS_BUTTON_H

#include "widget.h"

class FTPixmapFont;

/** @brief Button widget agent
 * */
class AButton : public AVWidget
{
    public:
	static const char* Type() { return "AButton";};
	static string PEType();
	AButton(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AButton();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
	virtual bool onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) override;
    protected:
	// From AVWidget
	virtual void Init() override;
    protected:
	// TODO to have shared font in visual env
	FTPixmapFont* mFont;
    protected:
	const int K_BFontSize = 18; /**< Base metric: Base font (unit name) size. */
	const int K_BPadding = 5; /**< Base metric: Base padding */
	const int K_LineWidth = 1; /**< Base metric: Line width */
};


#endif
