#include "visprov.h"
#include "visenv.h"
#include "scene.h"
#include "scelem.h"
#include "sce_rect.h"

VisProv::VisProv(const string &aName, MEnv* aEnv): GProvider(aName, aEnv)
{
     //g_type_init ();
}

Unit* VisProv::CreateNode(const string& aType, const string& aName, MUnit* aMan, MEnv* aEnv)
{
    Unit* res = NULL;
    if (aType.compare(AGWindow::Type()) == 0) {
	res = new AGWindow(aName, aMan, aEnv);
    } else if (aType.compare(AGtScene::Type()) == 0) {
	res = new AGtScene(aName, aMan, aEnv);
    } else if (aType.compare(AScElem::Type()) == 0) {
	res = new AScElem(aName, aMan, aEnv);
    } else if (aType.compare(ASceRect::Type()) == 0) {
	res = new ASceRect(aName, aMan, aEnv);
    } else if (aType.compare(AVisEnv::Type()) == 0) {
	res = new AVisEnv(aName, aMan, aEnv);
    }
    if (res != NULL) {
	Unit* parent = GetNode(aType);
	if (parent != NULL) {
	    MElem* eparent = parent->GetObj(eparent);
	    eparent->AppendChild(res);
	}
    }
    return res;
}

Unit* VisProv::GetNode(const string& aUri){
    MProvider* prov = iEnv->Provider();
    Unit* res = NULL;
    if (iReg.count(aUri) > 0) {
	res = iReg.at(aUri);
    }
    else { 
	Unit* parent = NULL;
	if (aUri.compare(AGWindow::Type()) == 0) {
	    parent = prov->GetNode("ADes");
	    res = new AGWindow(string(), NULL, iEnv);
	} else if (aUri.compare(AGtScene::Type()) == 0) {
	    parent = prov->GetNode("ADes");
	    res = new AGtScene(string(), NULL, iEnv);
	} else if (aUri.compare(AScElem::Type()) == 0) {
	    parent = prov->GetNode("ADes");
	    res = new AScElem(string(), NULL, iEnv);
	} else if (aUri.compare(ASceRect::Type()) == 0) {
	    parent = prov->GetNode("AScElem");
	    res = new ASceRect(string(), NULL, iEnv);
	} else if (aUri.compare(AVisEnv::Type()) == 0) {
	    parent = prov->GetNode("ADes");
	    res = new AVisEnv(string(), NULL, iEnv);
	}
	if (res != NULL) {
	    if (parent != NULL) {
		MElem* eparent = parent->GetObj(eparent);
		if (eparent != NULL) {
		    eparent->AppendChild(res);
		}
	    }
	    iReg.insert(TRegVal(aUri, res));
	}
    }
    return res;
}


void VisProv::AppendNodesInfo(vector<string>& aInfo)
{
    aInfo.push_back(AGWindow::Type());
    aInfo.push_back(AVisEnv::Type());
}

