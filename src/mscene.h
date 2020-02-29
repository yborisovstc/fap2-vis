#ifndef __FAP2VIS_MSCENE_H
#define __FAP2VIS_MSCENE_H

#include "mvisenv.h"

/** @brief Scene of Visial environment interface
 * */
class MScene: public MIface
{
    public:
	static const char* Type() { return "MScene";};
	virtual void RenderScene(void) = 0;
	/** @brief Cursor position handler
	 * @param aX, aY  cursor pos in window coordinates
	 * */
	virtual void onCursorPosition(double aX, double aY) = 0;
	virtual void onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods) = 0;
	// From MIface
	virtual MIface* MScene_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MScene_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MScene_Call(aSpec, aRes);}
	string Mid() const override { return MScene_Mid();}
};


#endif
