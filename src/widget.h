#ifndef __FAP2VIS_WIDGET_H
#define __FAP2VIS_WIDGET_H

#include <des.h>

#include <mscel.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>


/** @brief Widget base agent
 *
 * */
// TODO Use unit based DES agent as a base
class AVWidget : public ADes, public MSceneElem
{
    public:
	static const char* Type() { return "AVWidget";};
	static string PEType();
	AVWidget(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From AScElem
	virtual void Render() override;
	// From MDesSyncable
    public:
	virtual void Update();
    protected:
	virtual void Init();
    protected:
	bool mIsInitialised = false;
	GLuint mProgram;
	GLint mMvpLocation;
};

#endif // __FAP2VIS_WIDGET_H
