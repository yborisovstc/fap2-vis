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
	virtual Elem* CreateNode(const string& aType, const string& aName, MElem* aMan, MEnv* aEnv);
	virtual Elem* GetNode(const string& aUri);
	virtual void AppendNodesInfo(vector<string>& aInfo);
    private:
	vector<string> iNodesInfo;
};


#endif
