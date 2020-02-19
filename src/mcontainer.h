#ifndef __FAP2VIS_MCONTAINER_H
#define __FAP2VIS_MCONTAINER_H

#include <miface.h>


/** @brief Container slot
 * Represents container slot in connection to container component
 * */
class MVCslot: public MIface
{
    public:
	static const char* Type() { return "MVCslot";};
	//virtual MUnit* IfProv() = 0;
	// From MIface
	virtual MIface* MVCslot_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MVCslot_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MVCslot_Call(aSpec, aRes);}
	string Mid() const override { return MVCslot_Mid();}
};

#endif

