#ifndef __FAP2VIS_MWIDGET_H
#define __FAP2VIS_MWIDGET_H

#include <gdk/gdk.h>

class MGEventHandler
{
    public:
	static const char* Type() { return "MGEventHandler";};
	virtual void OnEvent(GdkEvent* event) = 0;
};

class MElem;

class MGWidget
{
    public:
	typedef enum {
	    EWs_Normal = 1,
	    EWs_Active,
	    EWs_Prelight,
	    EWs_Selected,
	    EWs_Insensitive
	} TState;
    public:
	static const char* Type() { return "MGWidget";};
	virtual MElem* GetLayoutHint() const = 0;
};

class MGWidgetOwner
{
    public:
	static const char* Type() { return "MGWidgetOwner";};
	virtual GdkWindow* GetWindow() = 0;
};

class MGWidgetComp
{
    public:
	static const char* Type() { return "MGWidgetComp";};
	virtual void OnGdkEvent(GdkEvent* event) = 0;
};

#endif

