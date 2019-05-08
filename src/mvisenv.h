#ifndef __FAP2VIS_MVISENV_H
#define __FAP2VIS_MVISENV_H


/** @brief Visial environment interface
 * */
class MVisEnv: public MIface
{
    public:
    typedef void (*TIdleHandler)();
    public:
	static const char* Type() { return "MVisEnv";};
	virtual void SetOnIdleHandler(TIdleHandler aHandler) = 0;
	virtual void Start(void) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
};

#endif
