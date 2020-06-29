
#ifndef __FAP2VIS_SYSTDRP_H
#define __FAP2VIS_SYSTDRP_H

#include "agentvr.h"

/** @brief System detail representation widget
 * */
class ASystDrp : public AUnitDrp
{
    public:
	static const char* Type() { return "ASystDrp";};
	static string PEType();
	ASystDrp(const string& aName = string(), MUnit* aMan = NULL, MEnv* aEnv = NULL);
	// From MSceneElem
	virtual void Render() override;
	// From MVrp
	virtual void SetCrtlBinding(const string& aCtrUri) override;
    protected:
	// From AUnitDrp
	virtual void CreateRp() override;
    protected:
	bool AreThereCompsRelToFirstColumn();
	void LayoutBodyColumn(MUnit* aColumn);
	/** @brief Checks if model has representation in given column
	 * @param aColumnPos position of the column to be checked
	 * @param aMdl model unit to be checked
	 * */
	bool HasCrpInColumn(TPos aColumnPos, MUnit* aMdl);
	/** @brief Checks if model has CRP
	 * @param aMdl model unit to be checked
	 * */
	bool HasCrp(MUnit* aMdl);
};

#endif
