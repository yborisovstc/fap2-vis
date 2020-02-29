
#include "scelem.h"


AScElem::AScElem(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AScElem::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AScElem::Update()
{
    ADes::Update();
}

MIface *AScElem::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MSceneElem::Type()) == 0) {
	res = dynamic_cast<MSceneElem*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

void AScElem::onSeCursorPosition(double aX, double aY)
{
}

void AScElem::getWndCoord(int aInpX, int aInpY, int& aOutX, int& aOutY)
{
}

void AScElem::onMouseButton(TFvButton aButton, TFvButtonAction aAction, int aMods)
{
}

