#include "visprov.h"
#include "visenv.h"
#include "scene.h"
#include "scelem.h"
#include "mdlvis.h"
#include "visrepr.h"
#include "sce_rect.h"
#include "widget.h"
#include "container.h"
#include "label.h"
#include "agentvr.h"
#include "vrcontroller.h"
#include "button.h"
#include "vlayout.h"
#include "alignment.h"
#include "containerl.h"
#include "vlayoutl.h"
#include "systdrp.h"


const string KModulesPath = "/usr/share/grayb/modules/";

/** Native agents factory registry */
const ProvBase::TFReg VisProv::mReg ( {
    Item<AVisEnv>(), Item<AScElem>(), Item<ASceRect>(), Item<AMdlVis>(), Item<AVisRepr>(), Item<AGWindow>(), Item<AGtScene>(),
    Item<AVWidget>(), Item<AVHLayout>(), Item<AVSlot>(), Item<AVCpsCpInp>(), Item<AVCpsCpOut>(), Item<AUnitCrp>(), Item<AVLabel>(),
    Item<AUnitDrp>(), Item<AVrController>(), Item<TrModelCreated>(), Item<AButton>(), Item<AVLayout>(), Item<AVDesLauncher>(),
    Item<AAlignment>(), Item<TrReqSum>(), Item<TrReqMax>(), Item<TrCursor>(), Item<TrCursorApplied>(), Item<AVrpView>(),
    Item<AVSlotL>(), Item<AVLayoutL>(), Item<AHLayoutL>(), Item<ASystDrp>()
});

VisProv::VisProv(const string& aName, MEnv* aEnv): ProvBase(aName, aEnv)
{
}

VisProv::~VisProv()
{
}

void VisProv::AppendNodesInfo(vector<string>& aInfo)
{
}

const string& VisProv::ModulesPath() const
{
    return KModulesPath;
}

