
#ifndef __FAP2VIS_AGENTVR_H
#define __FAP2VIS_AGENTVR_H

#include "widget.h"
#include "container.h"


/** @brief Agent visual representation widget
 * Base agent for agents representations
 * */
class AAgentVr : public AVWidget
{
    public:
	static const char* Type() { return "AAgentVr";};
	static string PEType();
	AAgentVr(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AAgentVr();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MSceneElem
	virtual void Render() override;
    protected:
	static void DrawLine(float x1, float y1, float x2, float y2);
    protected:
	const int K_BFontSize = 12; /**< Base metric: Base font (unit name) size. */
	const int K_BPadding = 3; /**< Base metric: Base padding */
	const int K_LineWidth = 1; /**< Base metric: Line width */
};

class FTPixmapFont;

/** @brief Unit compact representation widget
 * */
class AUnitCrp : public AAgentVr
{
    public:
	static const char* Type() { return "AUnitCrp";};
	static string PEType();
	AUnitCrp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AUnitCrp();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MSceneElem
	virtual void Render() override;
    protected:
	// From AVWidget
	virtual void Init() override;
    protected:
	// TODO to have shared font in visual env
	FTPixmapFont* mFont;
};


/** @brief Unit detail representation widget
 * */
class AUnitDrp : public AVHLayout
{
    public:
	static const char* Type() { return "AUnitDrp";};
	static string PEType();
	AUnitDrp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MSceneElem
	virtual void Render() override;
};


#endif

