#ifndef __FAP2VIS_PROV_H
#define __FAP2VIS_PROV_H

#include <prov.h>
#include <melem.h>
#include <menv.h>

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
