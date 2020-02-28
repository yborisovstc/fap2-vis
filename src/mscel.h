#ifndef __FAP2VIS_MSCEL_H
#define __FAP2VIS_MSCEL_H


/** @brief Scene element of Visial environment interface
 * */
class MSceneElem: public MIface
{
    public:
	static const char* Type() { return "MSceneElem";};
	virtual void Render() = 0;
	/** @brief Cursor position handler
	 * @param aX, aY  cursor pos in window coordinates
	 * */
	virtual void onSeCursorPosition(double aX, double aY) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
};


#endif
