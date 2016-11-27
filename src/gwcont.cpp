#include "gwcont.h"


// Container base

string AGCont::PEType()
{
    return AGWidget::PEType() + GUri::KParentSep + Type();
}

AGCont::AGCont(const string& aName, MElem* aMan, MEnv* aEnv): AGWidget(aName, aMan, aEnv)
{
}

AGCont::AGCont(MElem* aMan, MEnv* aEnv): AGWidget(Type(), aMan, aEnv)
{
}

void AGCont::Update()
{
    for (TInt ci = 0; ci < Host()->CompsCount(); ci++) {
	MElem* eit = Host()->GetComp(ci);
	MGWidget* comp = (MGWidget*) eit->GetSIfiC(MGWidget::Type(), this);
	if (comp != NULL) {
	   MElem* hint =  comp->GetLayoutHint();
	}
    }
}



// Container: Fixed layout

string AGContFixed::PEType()
{
    return AGCont::PEType() + GUri::KParentSep + Type();
}

AGContFixed::AGContFixed(const string& aName, MElem* aMan, MEnv* aEnv): AGCont(aName, aMan, aEnv)
{
}

AGContFixed::AGContFixed(MElem* aMan, MEnv* aEnv): AGCont(Type(), aMan, aEnv)
{
}

void AGContFixed::Update()
{
    for (TInt ci = 0; ci < Host()->CompsCount(); ci++) {
	MElem* eit = Host()->GetComp(ci);
	MGWidget* comp = (MGWidget*) eit->GetSIfiC(MGWidget::Type(), this);
	if (comp != NULL) {
	   MElem* hint =  comp->GetLayoutHint();
	}
    }
}

