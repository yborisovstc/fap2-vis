#ifndef __FAP2VIS_PROV_H
#define __FAP2VIS_PROV_H

#include <prov.h>
#include <melem.h>
#include <menv.h>

#if 0
// Default provider of the agents
//
class VisProv: public GProvider
{
    public:
	VisProv(const string &aName, MEnv* aEnv);
	// From MProvider
	virtual Unit* CreateNode(const string& aType, const string& aName, MUnit* aMan, MEnv* aEnv) override;
	virtual Unit* GetNode(const string& aUri) override;
	virtual void AppendNodesInfo(vector<string>& aInfo);
    private:
	vector<string> iNodesInfo;
};
#endif

class MUnit;
class Unit;

/** @brief Default provider
 * */
class VisProv: public ProvBase
{
    public:
	VisProv(const string& aName, MEnv* aEnv);
	virtual ~VisProv();
	virtual const TFReg& FReg() const {return mReg;}
	// From MProvider
	virtual void AppendNodesInfo(vector<string>& aInfo);
	virtual const string& ModulesPath() const;
    private:
	static const TFReg mReg;
};




#endif
