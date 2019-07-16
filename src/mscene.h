#ifndef __FAP2VIS_MSCENE_H
#define __FAP2VIS_MSCENE_H


/** @brief Scene of Visial environment interface
 * */
class MScene: public MIface
{
    public:
	static const char* Type() { return "MScene";};
	virtual void RenderScene(void) = 0;
	// From MIface
	virtual MIface* MScene_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MScene_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MScene_Call(aSpec, aRes);}
	string Mid() const override { return MScene_Mid();}
};


#endif
