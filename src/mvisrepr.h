#ifndef __FAP2VIS_MVISREPR_H
#define __FAP2VIS_MVISREPR_H

#include <miface.h>

class MIfProv;

/** @brief Visual representation interface
 *
 * The representation of the model
 * Main function of representation is creating implicit binding visual model
 * to physical model basing on assosiation rules
 * */
class MVisRepr: public MIface
{
    public:
	static const char* Type() { return "MVisRepr";};
	virtual MIfProv* IfProv() = 0;
	// From MIface
	virtual MIface* MVisRepr_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MVisRepr_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MVisRepr_Call(aSpec, aRes);}
	string Mid() const override { return MVisRepr_Mid();}
};


#endif
