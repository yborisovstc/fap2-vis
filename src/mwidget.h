#ifndef __FAP2VIS_MWIDGET_H
#define __FAP2VIS_MWIDGET_H


#include <miface.h>

/** @brief Container component
 * Represents container component in connection to container slot
 *
 * */
class MVCcomp: public MIface
{
    public:
	static const char* Type() { return "MVCcomp";};
	//virtual MUnit* IfProv() = 0;
	// From MIface
	virtual MIface* MVisRepr_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MVisRepr_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MVisRepr_Call(aSpec, aRes);}
	string Mid() const override { return MVisRepr_Mid();}
};

#endif

