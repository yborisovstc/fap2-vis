#include "visprov.h"
#include "widget.h"
#include "gwcont.h"

VisProv::VisProv(const string &aName, MEnv* aEnv): GProvider(aName, aEnv)
{
     //g_type_init ();
}

Elem* VisProv::CreateNode(const string& aType, const string& aName, MElem* aMan, MEnv* aEnv)
{
    Elem* res = NULL;
    if (aType.compare(AStateWnd::Type()) == 0) {
	res = new AStateWnd(aName, aMan, aEnv);
    } else if (aType.compare(AGehMediator::Type()) == 0) {
	res = new AGehMediator(aName, aMan, aEnv);
    } else if (aType.compare(AGWidget::Type()) == 0) {
	res = new AGWidget(aName, aMan, aEnv);
    } else if (aType.compare(AGWindow::Type()) == 0) {
	res = new AGWindow(aName, aMan, aEnv);
    } else if (aType.compare(AGCont::Type()) == 0) {
	res = new AGCont(aName, aMan, aEnv);
    } else if (aType.compare(AGContFixed::Type()) == 0) {
	res = new AGContFixed(aName, aMan, aEnv);
    } else if (aType.compare(AVisEnv::Type()) == 0) {
	res = new AVisEnv(aName, aMan, aEnv);
    }
    if (res != NULL) {
	Elem* parent = GetNode(aType);
	if (parent != NULL) {
	    parent->AppendChild(res);
	}
    }
    return res;
}

Elem* VisProv::GetNode(const string& aUri){
    MProvider* prov = iEnv->Provider();
    Elem* res = NULL;
    if (iReg.count(aUri) > 0) {
	res = iReg.at(aUri);
    }
    else { 
	Elem* parent = NULL;
	if (aUri.compare(AStateWnd::Type()) == 0) {
	    parent = prov->GetNode("Elem");
	    res = new AStateWnd(NULL, iEnv);
	} else if (aUri.compare(AGehMediator::Type()) == 0) {
	    parent = prov->GetNode("Elem");
	    res = new AGehMediator(NULL, iEnv);
	} else if (aUri.compare(AGWidget::Type()) == 0) {
	    parent = prov->GetNode("Elem");
	    res = new AGWidget(NULL, iEnv);
	} else if (aUri.compare(AGWindow::Type()) == 0) {
	    parent = prov->GetNode("AGWidget");
	    res = new AGWindow(NULL, iEnv);
	} else if (aUri.compare(AGCont::Type()) == 0) {
	    parent = prov->GetNode("AGWidget");
	    res = new AGCont(NULL, iEnv);
	} else if (aUri.compare(AGContFixed::Type()) == 0) {
	    parent = prov->GetNode("AGCont");
	    res = new AGContFixed(NULL, iEnv);
	} else if (aUri.compare(AVisEnv::Type()) == 0) {
	    parent = prov->GetNode("Elem");
	    res = new AVisEnv(NULL, iEnv);
	}
	if (res != NULL) {
	    if (parent != NULL) {
		parent->AppendChild(res);
	    }
	    iReg.insert(TRegVal(aUri, res));
	}
    }
    return res;
}


void VisProv::AppendNodesInfo(vector<string>& aInfo)
{
    aInfo.push_back(AGWidget::Type());
    aInfo.push_back(AGCont::Type());
    aInfo.push_back(AGContFixed::Type());
}

