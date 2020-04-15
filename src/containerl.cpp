
#include "containerl.h"

const string KWidgetRqsW_Name = "OutRqsW";
const string KWidgetRqsH_Name = "OutRqsH";
const string KCont_Padding = "Padding";

AVContainerL::AVContainerL(const string& aName, MUnit* aMan, MEnv* aEnv): AVWidget(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
    InsertContent(KCont_Padding);
}

string AVContainerL::PEType()
{
    return AVWidget::PEType() + GUri::KParentSep + Type();
}

MIface* AVContainerL::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    res = AVWidget::DoGetObj(aName);
    return res;
}

void AVContainerL::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    MUnit* host = iMan;
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    if (aName == MViewMgr::Type() && !aCtx.empty() && iMan->IsComp(aCtx.back())) {
	// For view manager redirect upward
	host = iMan->GetMan();
	rr = host->GetIfi(aName, ctx);
	InsertIfCache(aName, aCtx, host, rr);
    } else {
	AVWidget::UpdateIfi(aName, aCtx);
    }
}


// ==== AVSlot ====

const string KCompRqWExtd_Uri = "./InpRqsW/Int";
const string KCompRqHExtd_Uri = "./InpRqsH/Int";

AVSlotL::AVSlotL(const string& aName, MUnit* aMan, MEnv* aEnv): Syst(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AVSlotL::PEType()
{
    return Syst::PEType() + GUri::KParentSep + Type();
}

MIface* AVSlotL::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MVCslot::Type()) == 0) {
	res = dynamic_cast<MVCslot*>(this);
    } else {
	res = Syst::DoGetObj(aName);
    }
    return res;
}
