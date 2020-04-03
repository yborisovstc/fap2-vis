#ifndef __FAP2VIS_MVRCONTROLLER_H
#define __FAP2VIS_MVRCONTROLLER_H

#include <miface.h>

class MVrp;

/** @brief Model's visual representation controller interface
 * Manages main aspects of representation
 * */
class MVrController: public MIface
{
    public:
	static const char* Type() { return "MVrController";};
	virtual void CreateModel(const string& aSpecPath) = 0;
	virtual void OnRpSelected(const MVrp* aRp) = 0;
	virtual MUnit* ModelRoot() = 0;
	virtual void ApplyCursor(const string& aCursor) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
	virtual string Uid() const { return Mid() + "%" + Type();}
	virtual string MVrController_Mid() const { return "?";}
	virtual string Mid() const override { return MVrController_Mid();}
};

#endif
