#ifndef __FAP2VIS_MWINDOW_H
#define __FAP2VIS_MWINDOW_H


/** @brief Window interface
 * */
class MWindow: public MIface
{
    public:
	static const char* Type() { return "MWindow";};
	virtual void Start(void) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
};

#endif
