#ifndef __FAP2VIS_MVISENV_H
#define __FAP2VIS_MVISENV_H

/** @brief Button
 * */
typedef enum {
    EFvBtnLeft,
    EFvBtnRight,
} TFvButton;

/** @brief Button action
 * */
typedef enum {
    EFvBtnActPress,
    EFvBtnActRelease,
} TFvButtonAction;


/** @brief Visial environment interface
 * */
class MVisEnv: public MIface
{
    public:
	static const char* Type() { return "MVisEnv";};
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
};

#endif
