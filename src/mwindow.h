#ifndef __FAP2VIS_MWINDOW_H
#define __FAP2VIS_MWINDOW_H


/** @brief Window interface
 * */
class MWindow: public MIface
{
    public:
	static const char* Type() { return "MWindow";};
	virtual void Start(void) = 0;
	/** @brief Gets cursor position */
	virtual void GetCursorPos(double& aX, double& aY) = 0;
	/** @brief Gets size of window in pixels (actually framebuffer size) */
	virtual void GetFbSize(int* aW, int* aH) const = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
	virtual string Uid() const { return Mid() + "%" + Type();}
	virtual string MWindow_Mid() const { return "?";}
	virtual string Mid() const override { return MWindow_Mid();}
};

#endif
