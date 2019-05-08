#ifndef __FAP2VIS_SCERECT_H
#define __FAP2VIS_SCERECT_H

#include <scelem.h>


using namespace std;


/** @brier Element 'Rectangle' of Scene of GLUT base visualization module
 * */
class ASceRect: public AScElem
{
    public:
	static const char* Type() { return "ASceRect";};
	static string PEType();
	ASceRect(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From AScElem
	virtual void Render() override;
	// From MDesSyncable
    public:
	virtual void Update();
};

#endif // __FAP2VIS_SCELEM_H


