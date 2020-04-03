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


/** @brief Visual representation view interface
 * View is the part of standard view-model app architecture
 * This is upper-layer agent managing the view
 * */
class MViewMgr: public MIface, public MIfaceProv
{
    public:
	static const char* Type() { return "MViewMgr";};
	// From MIfaceProv
	MIface* DoGetIface(const string& aName) override { return MViewMgr_DoGetIface(aName);}
	virtual MIface* MViewMgr_DoGetIface(const string& aName) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
	virtual string Uid() const { return Mid() + "%" + Type();}
	virtual string MViewMgr_Mid() const { return "?";}
	virtual string Mid() const override { return MViewMgr_Mid();}
};

#endif

