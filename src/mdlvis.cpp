
#include "mdlvis.h"
#include "mvisrepr.h"


AMdlVis::AMdlVis(const string& aName, MUnit* aMan, MEnv* aEnv): Unit(aName, aMan, aEnv)
{
    iName = aName.empty() ? GetType(PEType()) : aName;
}

string AMdlVis::PEType()
{
    return Unit::PEType() + GUri::KParentSep + Type();
}

void AMdlVis::Construct()
{
}

MIface *AMdlVis::DoGetObj(const char *aName)
{
    MIface* res = NULL;
    if (strcmp(aName, MMdlVis::Type()) == 0) {
	res = dynamic_cast<MMdlVis*>(this);
    } else if (strcmp(aName, MAgent::Type()) == 0) {
	res = dynamic_cast<MAgent*>(this);
    } else {
	res = Unit::DoGetObj(aName);
    }
    return res;
}

MUnit* AMdlVis::IfProv()
{
    MUnit* res = dynamic_cast<MUnit*>(this);
    return res;
}

MIface* AMdlVis::MAgent_DoGetIface(const string& aUid)
{
    MIface* res = NULL;
    if (aUid == MUnit::Type())
	res = dynamic_cast<MUnit*>(this);
    return res;
}

void AMdlVis::UpdateIfi(const string& aName, const TICacheRCtx& aCtx)
{
    TIfRange rr;
    TICacheRCtx ctx(aCtx); ctx.push_back(this);
    MUnit* rq = (*aCtx.rbegin());
    MVisRepr* rqmv = rq->GetObj(rqmv);
    if (rqmv != NULL) {
	// Request from the base model representor
	// Redirect to visual model corresponding node
	// Find original model representation in the context
	MUnit* mdlu = NULL;
	MVisRepr* mdl = NULL;
	for (TICacheRCtx::const_reverse_iterator it = aCtx.rbegin(); it != aCtx.rend() && mdl == NULL; it++) {
	    mdlu = (*it);
	    mdl = mdlu->GetObj(mdl);
	}
	if (mdl != NULL) {
	    MUnit* rqru = (*aCtx.begin());
	    GUri ruri;
	    rqru->GetRUri(ruri, mdlu);
	    if (!ruri.IsErr()) {
		MUnit* rqrepr = GetNode(ruri);
		if (rqrepr != NULL) {
		    rr = rqrepr->GetIfi(aName, ctx);
		    InsertIfCache(aName, aCtx, rqrepr, rr);
		}
	    }
	}
    } else {
	// Request not directly from the base model representor
	if (aName == MMdlVis::Type()) {
	    Unit::UpdateIfi(aName, aCtx);
	} else {
	    MUnit* rq2 = (aCtx.size() > 1) ? *(aCtx.rbegin() + 1) : NULL;
	    MUnit* host = GetMan();
	    if (rq2 != NULL && host->IsComp(rq2)) {
		// Request from visual model
		MUnit* bcp = GetNode("./../MdlBinding");
		if (bcp != NULL) {
		    TIfRange rg = bcp->GetIfi(MVisRepr::Type(), this);
		    for (TIfIter it = rg.first; it != rg.second; it++) {
			MIface* repru = *it;
			MVisRepr* repr = dynamic_cast<MVisRepr*>(repru);
			MUnit* ipr = repr->IfProv();
			rr = ipr->GetIfi(aName, ctx);
			InsertIfCache(aName, aCtx, ipr, rr);
		    }
		}
	    }
	}
    }
}
#if 0
{
    TIfRange rr;
    TICacheRCtx ctx(aCtx);
    ctx.push_back(this);
    MIface* res = (MIface*) DoGetObj(aName.c_str());
    if (res != NULL) {
	InsertIfCache(aName, aCtx, this, res);
    } else {
	MUnit* bcp = GetNode("./../MdlBinding");
	if (bcp != NULL) {
	    TIfRange rg = bcp->GetIfi(MVisRepr::Type(), this);
	    for (TIfIter it = rg.first; it != rg.second; it++) {
		MVisRepr* repr = dynamic_cast<MVisRepr*>(*it);
		MUnit* ipr = repr->IfProv();
		rr = ipr->GetIfi(aName, ctx);
		InsertIfCache(aName, aCtx, ipr, rr);
	    }
	}
    }

    //Unit::UpdateIfi(aName, aCtx);
}
#endif
