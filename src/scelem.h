#ifndef __FAP2VIS_SCELEM_H
#define __FAP2VIS_SCELEM_H

#include <des.h>
#include <mvisenv.h>
#include <mscel.h>


using namespace std;


/** @brier Element of Scene of GLUT base visualization module
 * */
class AScElem: public ADes, public MSceneElem
{
    public:
	static const char* Type() { return "AScElem";};
	static string PEType();
	AScElem(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From AScElem
	virtual void Render() override {}
	// From MDesSyncable
    public:
	virtual void Update();
};

#endif // __FAP2VIS_SCELEM_H


