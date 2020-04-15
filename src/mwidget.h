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
	virtual MIface* MVCcomp_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MVCcomp_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MVCcomp_Call(aSpec, aRes);}
	string Mid() const override { return MVCcomp_Mid();}
	virtual string Uid() const { return Mid() + "%" + Type();}
};

#endif

