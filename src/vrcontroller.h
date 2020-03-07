
#ifndef __FAP2VIS_VRCONTROLLER_H
#define __FAP2VIS_VRCONTROLLER_H


#include <des.h>
#include <mdata.h>

#include "mvrcontroller.h"


class Env;
class MVrp;

/** @brief Model's visual representation controller
 *
 * */
class AVrController : public ADes, public MVrController, public MACompsObserver
{
    public:
	static const char* Type() { return "AVrController";};
	static string PEType();
	AVrController(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual ~AVrController();
	virtual MIface* DoGetObj(const char *aName) override;
	// From MVrController
	virtual string MVrController_Mid() const { return GetUri(NULL, true);}
	virtual void CreateModel(const string& aSpecPath) override;
	virtual void OnRpSelected(const MVrp* aRp) override;
	// From MACompsObserver
	virtual TBool HandleCompChanged(MUnit& aContext, MUnit& aComp, const string& aContName = string()) override;
	// From MUnit
	virtual void UpdateIfi(const string& aName, const TICacheRCtx& aCtx = TICacheRCtx()) override;
	// From MDesSyncable
	virtual void Update();
    protected:
	virtual void Init();
	MElem* GetSceneE();
	MVrp* GetDrp();
	MUnit* GetDrpU();
	MVrp* CreateDrp(const MUnit* aNode);
    protected:
	bool mIsInitialised = false;
	Env* mEnv;
};

/** @brief Transition of ModelCreated state */
class TrModelCreated: public ATrcBase, public MDVarGet, public MDtGet<Sdata<bool> >  {
    public:
	static const char* Type() { return "TrModelCreated";};
	static string PEType();
	TrModelCreated(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	virtual MIface* DoGetObj(const char *aName) override;
	// From MDVarGet
	virtual string VarGetIfid(){ return MDtGet<Sdata<bool> >::Type();}
	virtual void *DoGetDObj(const char *aName);
	// From MDtGet<Sdata<bool>>
	virtual void DtGet(Sdata<bool>& aData) override;
};

#endif
