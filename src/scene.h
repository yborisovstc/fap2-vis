#ifndef __FAP2VIS_SCENE_H
#define __FAP2VIS_SCENE_H

#include <des.h>
#include <mscene.h>


using namespace std;


/** @brier Scene of GLUT base visualization module
 * */
class AGtScene: public ADes, public MScene
{
    public:
	static const char* Type() { return "AGtScene";};
	static string PEType();
	AGtScene(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From Base
	virtual MIface *DoGetObj(const char *aName) override;
	// From MScene
	virtual void RenderScene(void) override;
    public:
	virtual void Update();
    protected:
	void Construct();
    protected:
	TBool mWndInit;
};

#endif // __FAP2VIS_SCENE_H


