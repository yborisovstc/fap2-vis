#ifndef __FAP2VIS_MMDLVIS_H
#define __FAP2VIS_MMDLVIS_H

#include <miface.h>

class MIfProv;
class MUnit;

/** @brief Model for visual representation
 *
 * */
class MMdlVis: public MIface
{
    public:
	static const char* Type() { return "MMdlVis";};
	virtual MUnit* IfProv() = 0;
	// From MIface
	virtual MIface* MVisRepr_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MVisRepr_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MVisRepr_Call(aSpec, aRes);}
	string Mid() const override { return MVisRepr_Mid();}
};


#endif
