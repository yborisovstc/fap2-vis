
#include "scene.h"

const string KWndCnt_Init = "Init";
const string KWndCnt_Init_Val = "Yes";

AGtScene::AGtScene(const string& aName, MUnit* aMan, MEnv* aEnv): ADes(aName, aMan, aEnv), mWndInit(EFalse)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AGtScene::PEType()
{
    return ADes::PEType() + GUri::KParentSep + Type();
}

void AGtScene::Construct()
{
}

MIface *AGtScene::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (aName == MScene::Type()) {
	res = dynamic_cast<MScene*>(this);
    } else {
	res = ADes::DoGetObj(aName);
    }
    return res;
}

void AGtScene::RenderScene(void)
{
}

void AGtScene::Update()
{
}


