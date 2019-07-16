#include "visprov.h"
#include "visenv.h"
#include "scene.h"
#include "scelem.h"
#include "mdlvis.h"
#include "visrepr.h"
#include "sce_rect.h"


const string KModulesPath = "/usr/share/grayb/modules/";

/** Native agents factory registry */
const ProvBase::TFReg VisProv::mReg ( {
    Item<AVisEnv>(), Item<AScElem>(), Item<ASceRect>(), Item<AMdlVis>(), Item<AVisRepr>(), Item<AGWindow>(), Item<AGtScene>()
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

