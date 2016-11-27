#ifndef __FAP2VIS_GWCONT_H
#define __FAP2VIS_GWCONT_H

#include "widget.h"

// Container base
class AGCont: public AGWidget
{
    public:
	static const char* Type() { return "AGCont";};
	static string PEType();
	AGCont(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AGCont(MElem* aMan = NULL, MEnv* aEnv = NULL);
	// From MDesSyncable
	virtual void Update();
};

// Container: fixed layout
class AGContFixed: public AGCont
{
    public:
	static const char* Type() { return "AGContFixed";};
	static string PEType();
	AGContFixed(const string& aName = string(), MElem* aMan = NULL, MEnv* aEnv = NULL);
	AGContFixed(MElem* aMan = NULL, MEnv* aEnv = NULL);
	// From MDesSyncable
	virtual void Update();
};

#endif
