#ifndef __FAP2VIS_MCONTAINER_H
#define __FAP2VIS_MCONTAINER_H

#include <miface.h>


/** @brief Container slot
 * Represents container slot in connection to container component
 * */
class MVCslot: public MIface
{
    public:
	static const char* Type() { return "MVCslot";};
	//virtual MUnit* IfProv() = 0;
	// From MIface
	virtual MIface* MVCslot_Call(const string& aSpec, string& aRes) { return NULL;}
	virtual string MVCslot_Mid() const {return "?";}
	MIface* Call(const string& aSpec, string& aRes) override {return MVCslot_Call(aSpec, aRes);}
	string Mid() const override { return MVCslot_Mid();}
};


/** @brief Visual representation view interface
 * View is the part of standard view-model app architecture
 * This is upper-layer agent managing the view
 * */
class MViewMgr: public MIface, public MIfaceProv
{
    public:
	static const char* Type() { return "MViewMgr";};
	// From MIfaceProv
	MIface* DoGetIface(const string& aName) override { return MViewMgr_DoGetIface(aName);}
	virtual MIface* MViewMgr_DoGetIface(const string& aName) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
	virtual string Uid() const { return Mid() + "%" + Type();}
	virtual string MViewMgr_Mid() const { return "?";}
	virtual string Mid() const override { return MViewMgr_Mid();}
};

class MUnit;

/** @brief Container interface
 * */
// TODO To verify the current design: is using of 'universal' position type valid?
// probably we need to have specific ifaces for layout variants (linear, 2d etc) where
// specific pos type is defined.
class MContainer: public MIface
{
    public:
	/** @brief Slot position type: 0..n - regular pos, -1 - after last */
	using TPos = pair<int, int>;
    public:
	static const char* Type() { return "MContainer";};
	/** @brief Creates widget of given type and name and then appends it
	 * @return Pointer to added widget unit
	 * */
	virtual MUnit* AddWidget(const string& aName, const string& aType, const string& aHint = string()) = 0;
	/** @brief Creates widget of given type and name and inserts it before given position
	 * @param  aPos position the widget to be inserted, -1 means 'before first position'
	 * @return Pointer to added widget unit
	 * */
	virtual MUnit* InsertWidget(const string& aName, const string& aType, const TPos& aPos) = 0;
	/** @brief Gets last slot postion */
	virtual TPos LastPos() const = 0;
	/** @brief Gets slot by slot postion */
	virtual MUnit* GetSlotByPos(const TPos& aPos) = 0;
	// From MIface
	virtual MIface* Call(const string& aSpec, string& aRes) override {return NULL;}
	virtual string Uid() const { return Mid() + "%" + Type();}
	virtual string Mid() const override { return MContainer_Mid();}
	virtual string MContainer_Mid() const { return "?";}
};


#endif

